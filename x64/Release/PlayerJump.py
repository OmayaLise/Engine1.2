import embeddedmodule
import datetime

def PlayerJump(inputs : embeddedmodule.Inputs, rb : embeddedmodule.Rigidbody, collider : embeddedmodule.Collider,  playerC : embeddedmodule.PlayerControler, delta : int) -> None :

	if (collider.m_hasCollide) :
		playerC.coyoteCountdown = 0;
		playerC.canJump = True
	elif (playerC.coyoteCountdown <= 0.2) :
		playerC.coyoteCountdown += delta;
		playerC.canJump = True
	else :
		playerC.canJump = False

	if (inputs.jump  == True and rb.velocity.y <= 0 and playerC.canJump == True) :
		rb.velocity +=  embeddedmodule.Vec3(0,12,0)
		rb.transform.translation += embeddedmodule.Vec3(0,0.1,0)

def Init() -> None:
	pass