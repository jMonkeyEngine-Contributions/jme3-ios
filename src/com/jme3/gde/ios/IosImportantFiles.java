/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.jme3.gde.ios;

import com.jme3.gde.core.importantfiles.ImportantFiles;
import java.util.ArrayList;
import org.netbeans.api.project.Project;
import org.openide.filesystems.FileObject;
import org.openide.loaders.DataObject;
import org.openide.loaders.DataObjectNotFoundException;
import org.openide.nodes.Node;
import org.openide.util.Exceptions;

/**
 *
 * @author normenhansen
 */
@org.openide.util.lookup.ServiceProvider(service = ImportantFiles.class)
public class IosImportantFiles implements ImportantFiles {

    @Override
    public Node[] getNodes(Project project) {
        ArrayList<Node> list = new ArrayList<Node>();
        try {
            FileObject mainAct = project.getProjectDirectory().getFileObject("ios/ios.properties");
            if (mainAct != null) {
                Node node = DataObject.find(mainAct).getNodeDelegate();
                node.setDisplayName("iOS Properties");
                list.add(node);
            }
            FileObject manif = project.getProjectDirectory().getFileObject("ios/src/JmeAppHarness.java");
            if (manif != null) {
                Node node = DataObject.find(manif).getNodeDelegate();
                node.setDisplayName("iOS Harness (java)");
                list.add(node);
            }
            FileObject buildProp = project.getProjectDirectory().getFileObject("ios/src/JmeAppHarness.m");
            if (buildProp != null) {
                Node node = DataObject.find(buildProp).getNodeDelegate();
                node.setDisplayName("iOS Harness (native)");
                list.add(node);
            }
        } catch (DataObjectNotFoundException ex) {
            Exceptions.printStackTrace(ex);
        }
        return list.toArray(new Node[list.size()]);
    }

    @Override
    public boolean hasFiles(Project proj) {
        if (proj.getProjectDirectory().getFileObject("ios/ios.properties") != null) {
            return true;
        }
        return false;
    }
}
