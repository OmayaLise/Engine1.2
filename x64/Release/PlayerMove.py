import embeddedmodule
import math

def PlayerMove(inputs : embeddedmodule.Inputs, rb : embeddedmodule.Rigidbody, transform : embeddedmodule.Transform, forwardCam : embeddedmodule.Vec3, rightCam : embeddedmodule.Vec3) -> None :
	move : embeddedmodule.Vec3 = embeddedmodule.Vec3(0, rb.velocity.y, 0)

	speed : float = 15
	YRotation: float
	if (forwardCam.x > 0 and forwardCam.z >= 0)  :
		YRotation = math.atan(forwardCam.z / forwardCam.x)
	elif (forwardCam.x > 0 and forwardCam.z < 0):
		YRotation = math.atan(forwardCam.z / forwardCam.x) + 2 * math.pi
	elif (forwardCam.x < 0) :
		YRotation = math.atan(forwardCam.z / forwardCam.x) + math.pi
	elif (forwardCam.x == 0 and forwardCam.z > 0) :
		YRotation = math.pi/2
	else :
		YRotation = -math.pi/2

	if (inputs.moveForward):
		move.x += speed * forwardCam.x
		move.z += speed * forwardCam.z
		transform.rotation.y = math.degrees(-YRotation) + 90

	if (inputs.moveBackward):
		move.x -= speed * forwardCam.x
		move.z -= speed * forwardCam.z
		transform.rotation.y = math.degrees(YRotation) + 270

	if (inputs.moveLeft):
		move.x -= speed * rightCam.x
		move.z -= speed * rightCam.z
		transform.rotation.y = math.degrees(-YRotation) + 180


	if (inputs.moveRight):
		move.x += speed * rightCam.x
		move.z += speed * rightCam.z
		transform.rotation.y = math.degrees(-YRotation)


	if (inputs.moveUp):
		move.y += speed

	if (inputs.moveDown):
		move.y -= speed

	rb.velocity = move

def Init() -> None:
	pass