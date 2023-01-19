import embeddedmodule

deathZoneHeightLimit : float = -15
startPosition : embeddedmodule.Vec3 = embeddedmodule.Vec3(p_x = 7, p_y = -3, p_z = 0)

def Update(transform : embeddedmodule.Transform) -> None :
	if (transform.translation.y < deathZoneHeightLimit) :
		transform.translation = startPosition

def Init() -> None:
	pass