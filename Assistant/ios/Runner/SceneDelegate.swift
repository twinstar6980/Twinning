import Flutter
import UIKit

class SceneDelegate: FlutterSceneDelegate {

  public override func scene(
    _ scene: UIScene,
    willConnectTo session: UISceneSession,
    options connectionOptions: UIScene.ConnectionOptions,
  ) -> Void {
    PlatformIntegrationManager.instance().inject_SceneDelegate_scene(self, scene, session, connectionOptions)
    return
  }

  public override func scene(
    _ scene: UIScene,
    openURLContexts URLContexts: Set<UIOpenURLContext>,
  ) -> Void {
    PlatformIntegrationManager.instance().inject_SceneDelegate_scene(self, scene, URLContexts)
    return
  }

}
