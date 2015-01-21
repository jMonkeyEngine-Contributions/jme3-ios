/*
 *  Copyright (c) 2009-2012 jMonkeyEngine
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 *  * Neither the name of 'jMonkeyEngine' nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.jme3.gde.ios.panel;

import com.jme3.gde.ios.IosTool;
import com.jme3.gde.core.j2seproject.ProjectExtensionManager;
import com.jme3.gde.core.j2seproject.ProjectExtensionProperties;
import com.jme3.gde.core.util.ZipExtensionTool;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JComponent;

import org.netbeans.api.project.Project;
import org.netbeans.spi.project.ui.support.ProjectCustomizer;
import org.openide.DialogDisplayer;
import org.openide.NotifyDescriptor;
import org.openide.filesystems.FileObject;
import org.openide.util.Exceptions;
import org.openide.util.Lookup;
import org.openide.util.NbBundle;

/**
 *
 * @author normenhansen
 */
@ProjectCustomizer.CompositeCategoryProvider.Registration(projectType = "org-netbeans-modules-java-j2seproject", category = "Application", position = 410)
public class IosCompositeProvider implements ProjectCustomizer.CompositeCategoryProvider {

    private static final Logger logger = Logger.getLogger(IosCompositeProvider.class.getName());
    private static final String CAT_MOBILE = "iOSDeployment"; // NOI18N
    private static ProjectExtensionProperties jwsProps = null;
    private String[] keyList = new String[]{
        "application.title",
        "main.class",
        "ios.enabled",
        "ios.version"
    };
    private String[] privateKeyList = new String[]{
        "ios.avian.path",
        "ios.dev.root"
    };

    public IosCompositeProvider() {
    }

    @Override
    public ProjectCustomizer.Category createCategory(Lookup context) {
        return ProjectCustomizer.Category.create(CAT_MOBILE,
                NbBundle.getMessage(IosCompositeProvider.class, "LBL_Category_iOS"), null);
    }

    @Override
    public JComponent createComponent(ProjectCustomizer.Category category, Lookup context) {
        jwsProps = new ProjectExtensionProperties(context.lookup(Project.class), keyList);
        IosCustomizerPanel panel = new IosCustomizerPanel(jwsProps);
        category.setStoreListener(new SavePropsListener(jwsProps, context.lookup(Project.class)));
        category.setOkButtonListener(panel);
        return panel;
    }

    private class SavePropsListener implements ActionListener {

        private String extensionName = "ios";
        private String extensionVersion = "v1.0";
        private String avianVersion = "v1.1.1";
        private String[] extensionDependencies = new String[]{"jar", "build-ios"};
        private ProjectExtensionManager manager = new ProjectExtensionManager(extensionName, extensionVersion, extensionDependencies);
        private ProjectExtensionProperties properties;
        private Project project;

        public SavePropsListener(ProjectExtensionProperties props, Project project) {
            this.properties = props;
            this.project = project;
            manager.setAntTaskLibrary("ios-base");
        }

        public void actionPerformed(ActionEvent e) {
            if (IosTool.getIosSdkVersions("iPhoneOS").isEmpty()) {
                showError("No XCode SDK could be found!\nPlease install XCode before proceeding.", null);
                return;
            }
            ProjectExtensionProperties privateProps = new ProjectExtensionProperties(project, privateKeyList, true);
            manager.setDataZip("nbres:/com/jme3/gde/ios/ios-data.zip");
            manager.setAntTaskLibrary("pro-guard");
            ZipExtensionTool zipExt = new ZipExtensionTool(IosTool.class.getPackage().getName(), "avian-openjdk", avianVersion);
            //zipExt.SUFFIX_LINUX = "mac";
            //zipExt.SUFFIX_WIN = "mac";

            if ("true".equals(properties.getProperty("ios.enabled"))) {
                zipExt.install(true);
                privateProps.setProperty("ios.avian.path", zipExt.getPath());
                privateProps.setProperty("ios.dev.root", IosTool.getIosDeveloperRootName());
                manager.loadTargets("nbres:/com/jme3/gde/ios/ios-targets.xml");
                manager.checkExtension(project);
                //manager.addRunConfiguration(project, "run-android", "Android Device", "run-android", "run-android", "clean clean-android");
                FileObject resFolder = project.getProjectDirectory().getFileObject("resources/ios");
                FileObject projFolder = resFolder.getFileObject("templates/project");
                FileObject srcFolder = resFolder.getFileObject("templates/src");
                FileObject propsFile = resFolder.getFileObject("templates/ios.properties");
                FileObject iosFolder = project.getProjectDirectory().getFileObject("ios");
                if (iosFolder == null) {
                    try {
                        iosFolder = project.getProjectDirectory().createFolder("ios");
                        projFolder.copy(iosFolder, projFolder.getName(), null);
                        propsFile.copy(iosFolder, propsFile.getName(), propsFile.getExt());
                        FileObject newSrcFolder = srcFolder.copy(iosFolder, srcFolder.getName(), null);
                        replaceHarnessData(newSrcFolder, properties);
                    } catch (IOException ex) {
                        showError("Error creating iOS folders", ex);
                        return;
                    }
                }
                project.getProjectDirectory().refresh();
            } else {
                manager.removeExtension(project);
                if ("true".equals(properties.getProperty("delete.folder"))) {
                    FileObject iosFolder = project.getProjectDirectory().getFileObject("ios");
                    if (iosFolder != null) {
                        try {
                            iosFolder.delete();
                        } catch (IOException ex) {
                            showError("Error deleting iosfolder", ex);
                            Exceptions.printStackTrace(ex);
                        }
                    }
                }
                project.getProjectDirectory().refresh();
            }
            try {
                properties.store();
                privateProps.store();
            } catch (IOException ioe) {
                showError("Error storing properties", ioe);
                Exceptions.printStackTrace(ioe);
            }
        }

        private void replaceHarnessData(FileObject newSrcFolder, ProjectExtensionProperties properties) throws IOException {
            //replace harness data
            FileObject appHarnessFile = newSrcFolder.getFileObject("JmeAppHarness.java");
            String text = appHarnessFile.asText();
            String mainClass = properties.getProperty("main.class");
            if (text == null || mainClass == null) {
                logger.log(Level.INFO, "Could not replace harness data in folder {0}, file {4}, text = [{1}], mainClass = [{2}]", new Object[]{newSrcFolder, text, mainClass, appHarnessFile});
                return;
            }
            text = text.replace("${main.class}", mainClass);
            OutputStream out = appHarnessFile.getOutputStream();
            BufferedWriter wrt = null;
            try {
                wrt = new BufferedWriter(new OutputStreamWriter(out));
                wrt.write(text, 0, text.length());
            } catch (IOException ey) {
                Exceptions.printStackTrace(ey);
            } finally {
                if (wrt != null) {
                    wrt.close();
                }
                if (out != null) {
                    out.close();
                }
            }
        }
    }

    private void showError(String message, Exception ex) {
        String mesg = message;
        if (ex != null) {
            Exceptions.printStackTrace(ex);
            mesg = mesg + "\n" + ex.getMessage();
        }
        NotifyDescriptor.Confirmation msg = new NotifyDescriptor.Confirmation(
                mesg,
                NotifyDescriptor.DEFAULT_OPTION,
                NotifyDescriptor.ERROR_MESSAGE);
        DialogDisplayer.getDefault().notify(msg);

    }
}
