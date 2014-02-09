import com.jme3.system.AppSettings;
import com.jme3.system.ios.IosHarness;
import com.jme3.renderer.ios.IGLESShaderRenderer;
import com.jme3.renderer.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * You can extend this class to perform iOS-only operations from java,
 * native methods can reside either in .c/.m files in this directory
 * or in the XCode project itself.
 * @author normenhansen
 */
public class JmeAppHarness extends IosHarness{

    private static final Logger logger = Logger.getLogger(JmeAppHarness.class.getName());
	protected IGLESShaderRenderer renderer;
	protected boolean autoFlush = true;


    /**
     * An instance of this object is created when your application
     * has started on the iOS side.
     * You can e.g. attach special AppStates or do similar things here. You can
     * access classes from this source directory as well as your main projects
     * sources and classpath.
     */
    public JmeAppHarness(long id) {
        super(id);
        app = new mygame.Main();
        AppSettings settings = new AppSettings(true);
        //settings.setRenderer(null);
        settings.setAudioRenderer(null);
        this.app.setSettings(settings);
        app.start();
        logger.log(Level.FINE, "JmeAppHarness constructor");
        app.gainFocus();
    }

    @Override
    public void appPaused() {
        logger.log(Level.FINE, "JmeAppHarness appPaused");
    }

    @Override
    public void appReactivated() {
        logger.log(Level.FINE, "JmeAppHarness appReactivated");
    }

    @Override
    public void appClosed() {
        logger.log(Level.FINE, "JmeAppHarness appClosed");
        app.stop();
    }

    @Override
    public void appUpdate() {
        logger.log(Level.FINE, "JmeAppHarness appUpdate");
       //app.update();
    }

    @Override
    public void appDraw() {
        logger.log(Level.FINE, "JmeAppHarness appDraw");
		if (renderer == null) {
			renderer = (IGLESShaderRenderer)app.getContext().getRenderer();
			renderer.initialize();
		} else {
			app.update();
    	    if (autoFlush) {
        	    renderer.onFrame();
        	}
        }
    }
    
    @Override
    public void appReshape(int width, int height) {
        logger.log(Level.FINE, "JmeAppHarness reshape");
        app.getContext().getSettings().setResolution(width, height);
        if (renderer != null) {
    		app.reshape(width, height);
    	}
    }
    
    /**
     * Example of a native method calling iOS code.
     * See the native code in IosHarness.m
     * @param text The message to display
     */
    public native void showDialog(String text);

}
