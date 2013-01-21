import com.jme3.system.AppSettings;
import com.jme3.system.ios.IosHarness;

/**
 * You can extend this class to perform iOS-only operations from java,
 * native methods can reside either in .c/.m files in this directory
 * or in the XCode project itself.
 * @author normenhansen
 */
public class JmeAppHarness extends IosHarness{

    /**
     * An instance of this object is created when your application
     * has started on the iOS side.
     * You can e.g. attach special AppStates or do similar things here. You can
     * access classes from this source directory as well as your main projects
     * sources and classpath.
     */
    public JmeAppHarness(long id) {
        super(id);
        app = new ${main.class}();
        AppSettings settings = new AppSettings(true);
        settings.setRenderer(null);
        settings.setAudioRenderer(null);
        this.app.setSettings(settings);
        app.start();
    }

    @Override
    public void appPaused() {
    }

    @Override
    public void appReactivated() {
    }

    @Override
    public void appClosed() {
        app.stop();
    }
    
    /**
     * Example of a native method calling iOS code.
     * See the native code in IosHarness.m
     * @param text The message to display
     */
    public native void showDialog(String text);

}
