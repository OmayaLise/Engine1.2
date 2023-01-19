import embeddedmodule

def InitGameObject() -> None :
    #PotatOs
    embeddedmodule.AddGameObjectInCurrentScene(
    "PotatOS", "BasicShader", "PotatOSText", 
    -1, -2, -5, 0.2, 0.2, 0.2, 0, 45, 0,
    "PotatOs", False)

    #Companion
    embeddedmodule.AddGameObjectInCurrentScene(
    "Companion", "BasicShader", "CompanionText", 
    -5, 3, 6, 0.1, 0.1, 0.1, 0, 0, 0,
    "Companion", False)

    #Slime
    embeddedmodule.AddGameObjectInCurrentScene(
    "Slime", "BasicShader", "SlimeText", 
    -23, 4, 11, 1, 1, 1, 0, 25, -22,
    "Slime", False)

    #Chocobo
    embeddedmodule.AddGameObjectInCurrentScene(
    "Chocobo", "BasicShader", "ChocoboText", 
    6, -4, 4, 0.02, 0.02, 0.02, 0, 150, -180,
    "Chocobo", False)

    #FryingPan
    embeddedmodule.AddGameObjectInCurrentScene(
    "FryingPan", "BasicShader", "FryingPanText", 
    -0.35, 0.45, -0.1, 0.025, 0.025, 0.025, 45, 90, 135,
    "FryingPan", False)

    #Pistol
    embeddedmodule.AddGameObjectInCurrentScene(
    "Pistol", "BasicShader", "PistolText", 
    .25, .3, -.1, .1, .1, .1, 0, 0, -35,
    "Pistol", False)

    #box1
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    0, -5, 0, 15, 1, 10, 0, 0, 0,
    "box1", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box2
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -30, -2, 4, 1.5, .5, 1.5, 0, 0, 0,
    "box2", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box3
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    0, -3, 0, 1, 1, 1, 0, 0, 0,
    "box3", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box5
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -6, 0, 1, 1, 1, 1, 0, 0, 0,
    "box5", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box6
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -9, 2, -5, 2, .5, 3, 0, 0, 0,
    "box6", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box7
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -15, 2, 6, 2, .5, 3, 0, 0, 0,
    "box7", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box8
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -25, 2, 0, 1.5, .5, 1.5, 0, 0, 0,
    "box8", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #box9
    embeddedmodule.AddGameObjectInCurrentScene(
    "Cube", "BasicShader", "Wall", 
    -20, -2, 0, 1.5, .5, 1.5, 0, 0, 0,
    "box9", False)
    embeddedmodule.CreateCollider("BoxCollider", "ColliderShader", 0, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)

    #Sphere
    embeddedmodule.AddGameObjectInCurrentScene(
    "Sphere", "BasicShader", "Wall", 
    -21, 4, -11, 4, 4, 4, 0, 0, 0,
    "Sphere", False)
    embeddedmodule.CreateCollider("SphereCollider", "ColliderShader", 2, True)
    embeddedmodule.SetLastColliderToLastGameObject(False)
    embeddedmodule.SetRadiusLastCollider(1.46)

    #Camera
    embeddedmodule.AddCamera()
    
    #Player
    embeddedmodule.AddGameObjectInCurrentScene(
    "Patrick", "BasicShader", "PatrickText", 
    7, 1, 0, 2, 2, 2, 0, 0, 0,
    "Player", True)
    embeddedmodule.CreateCollider("SphereCollider", "ColliderShader", 2, False)
    embeddedmodule.SetLastColliderToLastGameObject(True)
    embeddedmodule.SetRadiusLastCollider(.5)
    embeddedmodule.SetCenterLastCollider(0, .5, 0)
    embeddedmodule.SetPlayerLastGameObject()


def InitParent() -> None :
    embeddedmodule.GameObjectSetParent("Player", "Pistol")
    embeddedmodule.GameObjectSetParent("Player", "FryingPan")
    embeddedmodule.GameObjectSetParent("Player", "Camera")

def InitLight() -> None :
    embeddedmodule.AddDirectionLight(2, 1, 0, .3, .3, .3, 1, 1, 0, 0, 1, .9, .9, .9, 1, 0, 0, 1)
    embeddedmodule.AddPointLight(2, 1, 0, .3, .3, .3, 1, 1, 0, 0, 1, .9, .9, .9, 1, 1, .09, .0032)
    embeddedmodule.AddSpotLight(-2, 8, 0, .3, .3, .3, 1, 1, 0, 0, 1, .9, .9, .9, 1, 0, -1, 0, 12.5, 0.82)

def Init() -> None :
    InitGameObject()
    InitParent()
    InitLight()

def Update() -> None :
    pass