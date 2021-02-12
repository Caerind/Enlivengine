#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/EntityManager.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("TransformComponent - Simple")
{
	en::World world("Test");
	en::Entity entityA = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityA.IsValid());
	DOCTEST_CHECK(!entityA.Has<en::TransformComponent>());
	en::TransformComponent& transformComponentA = entityA.Add<en::TransformComponent>();
	DOCTEST_CHECK(entityA.Has<en::TransformComponent>());
	DOCTEST_CHECK(entityA == transformComponentA.GetEntity());
	en::TransformComponent& transformComponentAGet = entityA.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityA == transformComponentAGet.GetEntity());
	DOCTEST_CHECK(&transformComponentA == &transformComponentAGet);
}

DOCTEST_TEST_CASE("TransformComponent - Simple Children/Parent")
{
	en::World world("Test");

	en::Entity entityA = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityA.IsValid());
	en::Entity entityB = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityB.IsValid());
	DOCTEST_CHECK(entityA != entityB);

	// Creating many components can invalidate reference
	// Don't store references to components and use Get instead
	entityA.Add<en::TransformComponent>();
	entityB.Add<en::TransformComponent>();

	en::TransformComponent& transformComponentA = entityA.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityA == transformComponentA.GetEntity());
	DOCTEST_CHECK(entityB != transformComponentA.GetEntity());
	DOCTEST_CHECK(!transformComponentA.HasParent());
	DOCTEST_CHECK(transformComponentA.GetChildrenCount() == 0);

	en::TransformComponent& transformComponentB = entityB.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityB == transformComponentB.GetEntity());
	DOCTEST_CHECK(entityA != transformComponentB.GetEntity());
	DOCTEST_CHECK(!transformComponentB.HasParent());
	DOCTEST_CHECK(transformComponentB.GetChildrenCount() == 0);

	// Attach
	DOCTEST_CHECK(!transformComponentA.IsChild(entityB));
	DOCTEST_CHECK(transformComponentB.GetParent().Get() != entityA);
	transformComponentA.AttachChild(entityB);
	DOCTEST_CHECK(!transformComponentA.HasParent());
	DOCTEST_CHECK(transformComponentA.GetParent().Get() == en::Entity());
	DOCTEST_CHECK(transformComponentA.IsChild(entityB));
	DOCTEST_CHECK(transformComponentA.GetChildrenCount() == 1);
	DOCTEST_CHECK(transformComponentB.HasParent());
	DOCTEST_CHECK(transformComponentB.GetParent().Get() == entityA);
	DOCTEST_CHECK(!transformComponentB.IsChild(entityA));
	DOCTEST_CHECK(transformComponentB.GetChildrenCount() == 0);

	// Detach
	DOCTEST_CHECK(transformComponentA.IsChild(entityB));
	DOCTEST_CHECK(transformComponentB.GetParent().Get() == entityA);
	transformComponentA.DetachChild(entityB);
	DOCTEST_CHECK(!transformComponentA.HasParent());
	DOCTEST_CHECK(transformComponentA.GetParent().Get() == en::Entity());
	DOCTEST_CHECK(!transformComponentA.IsChild(entityB));
	DOCTEST_CHECK(transformComponentA.GetChildrenCount() == 0);
	DOCTEST_CHECK(!transformComponentB.HasParent());
	DOCTEST_CHECK(transformComponentB.GetParent().Get() != entityA);
	DOCTEST_CHECK(!transformComponentB.IsChild(entityA));
	DOCTEST_CHECK(transformComponentB.GetChildrenCount() == 0);
}

DOCTEST_TEST_CASE("TransformComponent - CanAttach")
{
	en::World world("Test");

	en::Entity entityA = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityA.IsValid());
	en::Entity entityB = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityB.IsValid());
	en::Entity entityC = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityC.IsValid());
	en::Entity entityD = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityD.IsValid());
	en::Entity entityE = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityE.IsValid());
	en::Entity entityF = world.GetEntityManager().CreateEntity();
	DOCTEST_CHECK(entityF.IsValid());

	// Creating many components can invalidate reference
	// Don't store references to components and use Get instead
	entityA.Add<en::TransformComponent>();
	entityB.Add<en::TransformComponent>();
	entityC.Add<en::TransformComponent>();
	entityD.Add<en::TransformComponent>();
	entityE.Add<en::TransformComponent>();

	en::TransformComponent& transformComponentA = entityA.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityA == transformComponentA.GetEntity());
	DOCTEST_CHECK(!transformComponentA.HasParent());
	DOCTEST_CHECK(transformComponentA.GetChildrenCount() == 0);

	en::TransformComponent& transformComponentB = entityB.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityB == transformComponentB.GetEntity());
	DOCTEST_CHECK(!transformComponentB.HasParent());
	DOCTEST_CHECK(transformComponentB.GetChildrenCount() == 0);

	en::TransformComponent& transformComponentC = entityC.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityC == transformComponentC.GetEntity());
	DOCTEST_CHECK(!transformComponentC.HasParent());
	DOCTEST_CHECK(transformComponentC.GetChildrenCount() == 0);

	en::TransformComponent& transformComponentD = entityD.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityD == transformComponentD.GetEntity());
	DOCTEST_CHECK(!transformComponentD.HasParent());
	DOCTEST_CHECK(transformComponentD.GetChildrenCount() == 0);

	en::TransformComponent& transformComponentE = entityE.Get<en::TransformComponent>();
	DOCTEST_CHECK(entityE == transformComponentE.GetEntity());
	DOCTEST_CHECK(!transformComponentE.HasParent());
	DOCTEST_CHECK(transformComponentE.GetChildrenCount() == 0);

	// A
	// - B
	//   - C
	// - D
	//   - E
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityF));
	DOCTEST_CHECK(transformComponentA.CanAttach(entityB));
	transformComponentA.AttachChild(entityB);
	DOCTEST_CHECK(transformComponentA.CanAttach(entityD));
	transformComponentA.AttachChild(entityD);
	DOCTEST_CHECK(transformComponentB.CanAttach(entityC));
	transformComponentB.AttachChild(entityC);
	DOCTEST_CHECK(transformComponentD.CanAttach(entityE));
	transformComponentD.AttachChild(entityE);


	DOCTEST_CHECK(!transformComponentA.CanAttach(entityA));
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityB));
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityC));
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityD));
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityE));
	DOCTEST_CHECK(!transformComponentA.CanAttach(entityF));

	DOCTEST_CHECK(transformComponentB.CanAttach(entityA));
	DOCTEST_CHECK(!transformComponentB.CanAttach(entityB));
	DOCTEST_CHECK(!transformComponentB.CanAttach(entityC));
	DOCTEST_CHECK(transformComponentB.CanAttach(entityD));
	DOCTEST_CHECK(transformComponentB.CanAttach(entityE));
	DOCTEST_CHECK(!transformComponentB.CanAttach(entityF));

	DOCTEST_CHECK(transformComponentC.CanAttach(entityA));
	DOCTEST_CHECK(transformComponentC.CanAttach(entityB));
	DOCTEST_CHECK(!transformComponentC.CanAttach(entityC));
	DOCTEST_CHECK(transformComponentC.CanAttach(entityD));
	DOCTEST_CHECK(transformComponentC.CanAttach(entityE));
	DOCTEST_CHECK(!transformComponentC.CanAttach(entityF));

	DOCTEST_CHECK(transformComponentD.CanAttach(entityA));
	DOCTEST_CHECK(transformComponentD.CanAttach(entityB));
	DOCTEST_CHECK(transformComponentD.CanAttach(entityC));
	DOCTEST_CHECK(!transformComponentD.CanAttach(entityD));
	DOCTEST_CHECK(!transformComponentD.CanAttach(entityE));
	DOCTEST_CHECK(!transformComponentD.CanAttach(entityF));

	DOCTEST_CHECK(transformComponentE.CanAttach(entityA));
	DOCTEST_CHECK(transformComponentE.CanAttach(entityB));
	DOCTEST_CHECK(transformComponentE.CanAttach(entityC));
	DOCTEST_CHECK(transformComponentE.CanAttach(entityD));
	DOCTEST_CHECK(!transformComponentE.CanAttach(entityE));
	DOCTEST_CHECK(!transformComponentE.CanAttach(entityF));
}