import com.jme3.app.Application;
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
     * @param app The application object that has been created
     */
    public JmeAppHarness(Application app) {
        super(app);
    }

    @Override
    public void appPaused() {
        super.appPaused();
    }

    @Override
    public void appReactivated() {
        super.appReactivated();
    }

    @Override
    public void appClosed() {
        super.appClosed();
    }
    
    /**
     * Example of a native method calling iOS code.
     * See the native code in IosHarness.m
     * @param text The message to display
     */
    public native void showDialog(String text);

}
