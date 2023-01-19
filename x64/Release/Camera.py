import embeddedmodule
import math

radiusPlayer : float = 5

def UpdateInputs(inputs : embeddedmodule.Inputs, transformCam : embeddedmodule.Transform, rotationAroundPlayer : embeddedmodule.Vec3, transformParent : embeddedmodule.Transform, 
				 gizmo : embeddedmodule.Transform) -> None :

	rotationAroundPlayer.y += inputs.deltaX / 500
	rotationAroundPlayer.x += inputs.deltaY / 6
	
	if (rotationAroundPlayer.y < -360) :
		rotationAroundPlayer.y = 360

	if (rotationAroundPlayer.y > 360) :
		rotationAroundPlayer.y = -360

	if (rotationAroundPlayer.x < -15) :
		rotationAroundPlayer.x = -15

	if (rotationAroundPlayer.x > 15) :
		rotationAroundPlayer.x = 15

	newFront : embeddedmodule.Vec3 = -(transformCam.translation - transformParent.translation)
	
	gizmo.translation = newFront.Normalize()
	# also re-calculate the Right and Up vector
	gizmo.scale = gizmo.translation.CrossProduct(embeddedmodule.Vec3(0,1,0)).Normalize()  
	# normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	gizmo.rotation = gizmo.scale.CrossProduct(gizmo.translation).Normalize()


def CalculateViewMatrix(transformCam : embeddedmodule.Transform, gizmo : embeddedmodule.Transform) -> embeddedmodule.Mat4 :
	return embeddedmodule.LookAt(transformCam.translation, transformCam.translation + gizmo.translation, gizmo.rotation)


def Update(inputs : embeddedmodule.Inputs, transformCam : embeddedmodule.Transform, 
		   rotationAroundPlayer : embeddedmodule.Vec3, transformParent : embeddedmodule.Transform, 
		   gizmo : embeddedmodule.Transform, transformMatrix : embeddedmodule.Transform) -> None :
	
	transformCam.translation = embeddedmodule.Vec3(math.cos(rotationAroundPlayer.y) * radiusPlayer, 4, math.sin(rotationAroundPlayer.y) * radiusPlayer) + transformParent.translation
	UpdateInputs(inputs, transformCam, rotationAroundPlayer, transformParent, gizmo)
	
	transformMatrix.matrix = CalculateViewMatrix(transformCam, gizmo)

def Init() -> None:
	pass