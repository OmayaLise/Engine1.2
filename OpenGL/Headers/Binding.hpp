#pragma once

// Python
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/operators.h>

#include "Log.hpp"

void AddGameObjectInCurrentScene(const std::string& p_mesh, const std::string& p_shader, const std::string& p_texture, 
	const float p_positionX, const float p_positionY, const float p_positionZ, 
	const float p_scaleX, const float p_scaleY, const float p_scaleZ, 
	const float p_rotationX, const float p_rotationY, const float p_rotationZ, 
	const std::string& p_name, const bool p_isPlayer)
{
	Core::App* app = Core::App::GetInstance();

	LowRenderer::Model model = app->CreateModel(p_mesh, p_shader, p_texture);

	LowRenderer::GameObject* gameObject = new LowRenderer::GameObject(model, Physics::Transform(
		Core::Maths::Vec3(p_positionX, p_positionY, p_positionZ),
		Core::Maths::Vec3(p_scaleX, p_scaleY, p_scaleZ),
		Core::Maths::Vec3(p_rotationX, p_rotationY, p_rotationZ)), p_name);

	if(p_isPlayer)
		app->AddGameObject(gameObject, LowRenderer::GOType::Player);
	else
		app->AddGameObject(gameObject);
}

// TODO : change p_colliderType
void CreateCollider(const std::string p_mesh, const std::string p_shader, const int p_colliderType, const bool p_isStatic)
{
	Core::App* app = Core::App::GetInstance();

	app->currentScene->CreateCollider(app->resources.GetResource<Resources::Mesh>(p_mesh), app->resources.GetResource<Resources::Shader>(p_shader), (Physics::ColliderTypes)p_colliderType, p_isStatic);
}

void SetLastColliderToLastGameObject(const bool p_useGravity)
{
	Core::App* app = Core::App::GetInstance();

	app->currentScene->GetLastGameObject()->SetCollider(app->currentScene->GetLastCollider());
	app->currentScene->GetLastGameObject()->GetRigidbody().useGravity = p_useGravity;
}

void GameObjectSetParent(const std::string& p_parent, const std::string& p_child)
{
	Core::App* app = Core::App::GetInstance();

	Assertion(app->currentScene->SetParent(p_parent, p_child), "Fail to set parent");
}

void AddCamera()
{
	Core::App* app = Core::App::GetInstance();

	LowRenderer::Camera* camera = new LowRenderer::Camera();
	app->currentScene->AddGameObject(camera, LowRenderer::GOType::Camera);
}

void SetPlayerLastGameObject()
{
	Core::App* app = Core::App::GetInstance();

	app->currentScene->GetLastGameObject()->SetEnablePlayerControler(true);
}

void SetRadiusLastCollider(const float p_radius)
{
	Core::App* app = Core::App::GetInstance();

	Physics::Collider* collider = app->currentScene->GetLastCollider();
	Physics::SphereCollider * sphereCollider = dynamic_cast<Physics::SphereCollider*>(collider);
	sphereCollider->radius = p_radius;
}

void SetCenterLastCollider(const float p_x, const float p_y, const float p_z)
{
	Core::App* app = Core::App::GetInstance();

	Physics::Collider* collider = app->currentScene->GetLastCollider();
	Physics::SphereCollider* sphereCollider = dynamic_cast<Physics::SphereCollider*>(collider);
	sphereCollider->center = {p_x, p_y, p_z};
}

void AddDirectionLight(const float& p_positionX, const float& p_positionY, const float& p_positionZ,
	const float& p_ambientColorR, const float& p_ambientColorG, const float& p_ambientColorB, const float& p_ambientColorA, 
	const float& p_diffuseColorR, const float& p_diffuseColorG, const float& p_diffuseColorB, const float& p_diffuseColorA, 
	const float& p_specularColorR, const float& p_specularColorG, const float& p_specularColorB, const float& p_specularColorA, 
	const float& p_directionX, const float& p_directionY, const float& p_directionZ)
{
	Core::App* app = Core::App::GetInstance();

	LowRenderer::InitLight initLight
	{
		Core::Maths::Vec3(p_positionX, p_positionY, p_positionZ),	   // position
		Core::Maths::Vec4(p_ambientColorR, p_ambientColorG, p_ambientColorB, p_ambientColorA), // ambientColor
		Core::Maths::Vec4(p_diffuseColorR, p_diffuseColorG, p_diffuseColorB, p_diffuseColorA), // diffuseColor
		Core::Maths::Vec4(p_specularColorR, p_specularColorG, p_specularColorB, p_specularColorA)  // specularColor 
	};

	app->currentScene->AddDirectionLight(LowRenderer::DirectionLight(initLight, Core::Maths::Vec3(p_directionX, p_directionY, p_directionZ)));
}

void AddPointLight(const float& p_positionX, const float& p_positionY, const float& p_positionZ,
	const float& p_ambientColorR, const float& p_ambientColorG, const float& p_ambientColorB, const float& p_ambientColorA,
	const float& p_diffuseColorR, const float& p_diffuseColorG, const float& p_diffuseColorB, const float& p_diffuseColorA,
	const float& p_specularColorR, const float& p_specularColorG, const float& p_specularColorB, const float& p_specularColorA,
	const float& p_constant, const float& p_linear, const float& p_quadratic)
{
	Core::App* app = Core::App::GetInstance();

	LowRenderer::InitLight initLight
	{
		Core::Maths::Vec3(p_positionX, p_positionY, p_positionZ),	   // position
		Core::Maths::Vec4(p_ambientColorR, p_ambientColorG, p_ambientColorB, p_ambientColorA), // ambientColor
		Core::Maths::Vec4(p_diffuseColorR, p_diffuseColorG, p_diffuseColorB, p_diffuseColorA), // diffuseColor
		Core::Maths::Vec4(p_specularColorR, p_specularColorG, p_specularColorB, p_specularColorA)  // specularColor 
	};

	app->currentScene->AddPointLight(LowRenderer::PointLight(initLight, p_constant, p_linear, p_quadratic));
}

void AddSpotLight(const float& p_positionX, const float& p_positionY, const float& p_positionZ,
	const float& p_ambientColorR, const float& p_ambientColorG, const float& p_ambientColorB, const float& p_ambientColorA,
	const float& p_diffuseColorR, const float& p_diffuseColorG, const float& p_diffuseColorB, const float& p_diffuseColorA,
	const float& p_specularColorR, const float& p_specularColorG, const float& p_specularColorB, const float& p_specularColorA,
	const float& p_directionX, const float& p_directionY, const float& p_directionZ,
	const float& p_cutOff, const float& p_outerCutOff)
{
	Core::App* app = Core::App::GetInstance();

	LowRenderer::InitLight initLight
	{
		Core::Maths::Vec3(p_positionX, p_positionY, p_positionZ),	   // position
		Core::Maths::Vec4(p_ambientColorR, p_ambientColorG, p_ambientColorB, p_ambientColorA), // ambientColor
		Core::Maths::Vec4(p_diffuseColorR, p_diffuseColorG, p_diffuseColorB, p_diffuseColorA), // diffuseColor
		Core::Maths::Vec4(p_specularColorR, p_specularColorG, p_specularColorB, p_specularColorA)  // specularColor 
	};

	app->currentScene->AddSpotLight(LowRenderer::SpotLight(initLight, Core::Maths::Vec3(p_directionX, p_directionY, p_directionZ), p_cutOff, p_outerCutOff));
}

PYBIND11_EMBEDDED_MODULE(embeddedmodule, module)
{
	module.doc() = "Embedded Module";
	module.def("AddGameObjectInCurrentScene", &AddGameObjectInCurrentScene);
	module.def("CreateCollider", &CreateCollider);
	module.def("SetLastColliderToLastGameObject", &SetLastColliderToLastGameObject);
	module.def("GameObjectSetParent", &GameObjectSetParent);
	module.def("SetRadiusLastCollider", &SetRadiusLastCollider);
	module.def("SetCenterLastCollider", &SetCenterLastCollider);
	module.def("SetPlayerLastGameObject", &SetPlayerLastGameObject);
	module.def("AddDirectionLight", &AddDirectionLight);
	module.def("AddPointLight", &AddPointLight);
	module.def("AddSpotLight", &AddSpotLight);
	module.def("AddCamera", &AddCamera);

	pybind11::class_<Core::Maths::Vec3>(module, "Vec3")
		.def(pybind11::init<const float&, const float&, const float&>(), "Constructor", pybind11::arg("p_x"), pybind11::arg("p_y"), pybind11::arg("p_z"))
		.def(pybind11::init<const Core::Maths::Vec3&>(), "ConstructorCopy", pybind11::arg("p_vec3"))
		.def_readwrite("x", &Core::Maths::Vec3::x)
		.def_readwrite("y", &Core::Maths::Vec3::y)
		.def_readwrite("z", &Core::Maths::Vec3::z)
		.def("Normalize", &Core::Maths::Vec3::Normalize)
		.def("CrossProduct", &Core::Maths::Vec3::CrossProduct)
		.def(pybind11::self + pybind11::self)
		.def(pybind11::self - pybind11::self)
		.def(-pybind11::self)
		.def(pybind11::self += pybind11::self)
		;

	pybind11::class_<Physics::Rigidbody>(module, "Rigidbody")
		.def_readwrite("velocity", &Physics::Rigidbody::velocity)
		.def_readwrite("gravityVelocity", &Physics::Rigidbody::gravityVelocity)
		.def_readwrite("angularVelocity", &Physics::Rigidbody::angularVelocity)
		.def_readwrite("transform", &Physics::Rigidbody::m_transform)
		;

	pybind11::class_<Core::Inputs>(module, "Inputs")
		.def_readwrite("moveForward", &Core::Inputs::moveForward)
		.def_readwrite("moveBackward", &Core::Inputs::moveBackward)
		.def_readwrite("moveLeft", &Core::Inputs::moveLeft)
		.def_readwrite("moveRight", &Core::Inputs::moveRight)
		.def_readwrite("moveUp", &Core::Inputs::moveUp)
		.def_readwrite("moveDown", &Core::Inputs::moveDown)
		.def_readwrite("deltaX", &Core::Inputs::deltaX)
		.def_readwrite("deltaY", &Core::Inputs::deltaY)
		.def_readwrite("jump", &Core::Inputs::jump)
		;

	pybind11::class_<Physics::Transform>(module, "Transform")
		.def_readwrite("translation", &Physics::Transform::translation)
		.def_readwrite("scale", &Physics::Transform::scale)
		.def_readwrite("rotation", &Physics::Transform::rotation)
		.def_readwrite("matrix", &Physics::Transform::matrix)
		;

	pybind11::class_<Core::Maths::Mat4>(module, "Mat4")
		.def(pybind11::init<const Core::Maths::Mat4&>(), "ConstuctorCopy", pybind11::arg("p_mat4"))
		.def("Display", &Core::Maths::Mat4::Display)
		;

	module.def("LookAt", &LowRenderer::Camera::LookAt);
	pybind11::class_<Physics::Collider>(module, "Collider")
		.def_readwrite("m_hasCollide", &Physics::Collider::m_hasCollide)
		;

	pybind11::class_<Gameplay::Player::PlayerControler>(module, "PlayerControler")
		.def_readwrite("canJump", &Gameplay::Player::PlayerControler::canJump)
		.def_readwrite("coyoteCountdown", &Gameplay::Player::PlayerControler::coyoteCountdown)
		;

}