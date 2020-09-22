#pragma once

#include <Box2D/Box2D.h>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

#include <Enlivengine/Core/CustomTraits.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Core/ObjectEditor.hpp> // TODO : Should be moved elsewhere to have clear Components

namespace en
{

enum class PhysicBodyType
{
	Dynamic,
	Static,
	Kinematic
};
b2BodyType ToB2BodyType(PhysicBodyType type);
PhysicBodyType FromB2BodyType(b2BodyType type);

enum class PhysicShapeType
{
	Circle,
	Edge,
	Polygon,
	Chain
};
b2Shape::Type ToB2ShapeType(PhysicShapeType type);
PhysicShapeType FromB2ShapeType(b2Shape::Type type);

class PhysicComponent
{
    public:
		PhysicComponent();
		PhysicComponent(PhysicComponent&& other) noexcept;
		PhysicComponent(const PhysicComponent&) = delete;
		~PhysicComponent();

		PhysicComponent& operator=(PhysicComponent&& other) noexcept;
		PhysicComponent& operator=(const PhysicComponent&) = delete;

		bool IsValid() const;
        
        void SetBodyType(PhysicBodyType type);
		PhysicBodyType GetBodyType() const;
        
        void SetLinearVelocity(const Vector2f& velocity);
        Vector2f GetLinearVelocity() const;
        
        void SetAngularVelocity(F32 velocity);
        F32 GetAngularVelocity() const;
        
        void SetGravityScale(F32 scale);
        F32 GetGravityScale() const;
        
        void SetLinearDamping(F32 value);
        F32 GetLinearDamping() const;
        
        void SetAngularDamping(F32 value);
        F32 GetAngularDamping() const;
        
        void SetFixedRotation(bool value);
        bool IsFixedRotation() const;
        
        void SetBullet(bool value);
        bool IsBullet() const;

		F32 GetMass() const;
        
        // TODO : AllowSleeping/Awake/Enable
        
        // TODO : Fixtures

		b2Body* GetBody();
		const b2Body* GetBody() const;

		Entity GetEntity() const;
		World* GetWorld();
		const World* GetWorld() const;

    private:
		friend class PhysicSystem;

		Entity mEntity;
        b2Body* mBody;
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO(en::PhysicBodyType)
ENLIVE_DEFINE_TYPE_INFO(en::PhysicShapeType)

ENLIVE_META_CLASS_BEGIN(en::PhysicComponent)
	ENLIVE_META_CLASS_MEMBER("type", &en::PhysicComponent::GetBodyType, &en::PhysicComponent::SetBodyType),
	ENLIVE_META_CLASS_MEMBER("linearVelocity", &en::PhysicComponent::GetLinearVelocity, &en::PhysicComponent::SetLinearVelocity),
	ENLIVE_META_CLASS_MEMBER("angularVelocity", &en::PhysicComponent::GetAngularVelocity, &en::PhysicComponent::SetAngularVelocity),
	ENLIVE_META_CLASS_MEMBER("gravityScale", &en::PhysicComponent::GetGravityScale, &en::PhysicComponent::SetGravityScale),
	ENLIVE_META_CLASS_MEMBER("linearDamping", &en::PhysicComponent::GetLinearDamping, &en::PhysicComponent::SetLinearDamping),
	ENLIVE_META_CLASS_MEMBER("angularDamping", &en::PhysicComponent::GetAngularDamping, &en::PhysicComponent::SetAngularDamping),
	ENLIVE_META_CLASS_MEMBER("fixedRotation", &en::PhysicComponent::IsFixedRotation, &en::PhysicComponent::SetFixedRotation),
	ENLIVE_META_CLASS_MEMBER("bullet", &en::PhysicComponent::IsBullet, &en::PhysicComponent::SetBullet)
ENLIVE_META_CLASS_END()

template <>
struct CustomObjectEditor<en::PhysicComponent>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::PhysicComponent& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();
			if (ImGui::CollapsingHeader("Body"))
			{
				ImGui::Indent();
				auto bType = object.GetBodyType();
				if (en::ObjectEditor::ImGuiEditor(bType, "BodyType"))
				{
					object.SetBodyType(bType);
					modified = true;
				}
				if (bType == en::PhysicBodyType::Dynamic || bType == en::PhysicBodyType::Kinematic)
				{
					auto linearVelocity = object.GetLinearVelocity();
					if (en::ObjectEditor::ImGuiEditor(linearVelocity, "LinearVelocity"))
					{
						object.SetLinearVelocity(linearVelocity);
						modified = true;
					}
					auto angularVelocity = object.GetAngularVelocity();
					if (en::ObjectEditor::ImGuiEditor(angularVelocity, "AngularVelocity"))
					{
						object.SetAngularVelocity(angularVelocity);
						modified = true;
					}
					auto gravityScale = object.GetGravityScale();
					if (en::ObjectEditor::ImGuiEditor(gravityScale, "GravityScale"))
					{
						object.SetGravityScale(gravityScale);
						modified = true;
					}
					auto linearDamping = object.GetLinearDamping();
					if (en::ObjectEditor::ImGuiEditor(linearDamping, "LinearDamping"))
					{
						object.SetLinearDamping(linearDamping);
						modified = true;
					}
					auto angularDamping = object.GetAngularDamping();
					if (en::ObjectEditor::ImGuiEditor(angularDamping, "AngularDamping"))
					{
						object.SetAngularDamping(angularDamping);
						modified = true;
					}
					auto fixedRotation = object.IsFixedRotation();
					if (en::ObjectEditor::ImGuiEditor(fixedRotation, "FixedRotation"))
					{
						object.SetFixedRotation(fixedRotation);
						modified = true;
					}
					auto bullet = object.IsBullet();
					if (en::ObjectEditor::ImGuiEditor(bullet, "Bullet"))
					{
						object.SetBullet(bullet);
						modified = true;
					}
				}
				ImGui::Unindent();
			}
			if (ImGui::CollapsingHeader("Fixtures"))
			{
				ImGui::Indent();
				auto* body = object.GetBody();

				ImGui::Text("Total mass : %f", body->GetMass());

				if (ImGui::Button("Add new fixture"))
				{
					b2FixtureDef newFixtureDef;
					newFixtureDef.density = 1.0f;
					newFixtureDef.isSensor = false;
					b2CircleShape circleShape;
					circleShape.m_radius = 1.0f;
					circleShape.m_p.Set(0.0f, 0.0f);
					newFixtureDef.shape = &circleShape;
					body->CreateFixture(&newFixtureDef);
				}

				en::U32 fixtureID = 0;
				for (b2Fixture* fixture = body->GetFixtureList(); fixture; )
				{
					if (fixture != nullptr)
					{
						ImGui::Separator();
						ImGui::PushID(fixtureID);

						b2Shape* shape = fixture->GetShape();

						bool shapeChanged = false;
						en::PhysicShapeType shapeType = en::FromB2ShapeType(shape->GetType());
						if (en::ObjectEditor::ImGuiEditor(shapeType, "ShapeType"))
						{
							shapeChanged = true;
							modified = true;

							b2FixtureDef newFixtureDef;
							newFixtureDef.density = fixture->GetDensity();
							newFixtureDef.friction = fixture->GetFriction();
							newFixtureDef.restitution = fixture->GetRestitution();
							newFixtureDef.isSensor = fixture->IsSensor();

							switch (shapeType)
							{
							case en::PhysicShapeType::Circle:
							{
								b2CircleShape circleShape;
								circleShape.m_radius = 1.0f;
								circleShape.m_p.Set(0.0f, 0.0f);
								newFixtureDef.shape = &circleShape;
								body->CreateFixture(&newFixtureDef);
							} break;
							case en::PhysicShapeType::Edge:
							{
								b2EdgeShape edgeShape;
								edgeShape.m_vertex1.Set(-1.0f, 0.0f);
								edgeShape.m_vertex2.Set(1.0f, 0.0f);
								edgeShape.m_vertex0.Set(-2.0f, 0.0f);
								edgeShape.m_vertex3.Set(2.0f, 0.0f);
								edgeShape.m_hasVertex0 = false;
								edgeShape.m_hasVertex3 = false;
								newFixtureDef.shape = &edgeShape;
								body->CreateFixture(&newFixtureDef);
							} break;
							case en::PhysicShapeType::Polygon:
							{
								b2PolygonShape polygonShape;
								polygonShape.SetAsBox(0.5f, 0.5f);
								newFixtureDef.shape = &polygonShape;
								body->CreateFixture(&newFixtureDef);
							} break;
							case en::PhysicShapeType::Chain:
							{
								b2ChainShape chainShape;
								b2Vec2 vertices[2];
								vertices[0].Set(-1.0f, 0.0f);
								vertices[1].Set(1.0f, 0.0f);
								chainShape.CreateChain(vertices, 2);
								chainShape.m_prevVertex.Set(-2.0f, 0.0f);
								chainShape.m_nextVertex.Set(2.0f, 0.0f);
								chainShape.m_hasPrevVertex = false;
								chainShape.m_hasNextVertex = false;
								newFixtureDef.shape = &chainShape;
								body->CreateFixture(&newFixtureDef);
							} break;
							default: enAssert(false);
							}

							b2Fixture* temp = fixture;
							fixture = fixture->GetNext();
							body->DestroyFixture(temp);
						}
						else
						{
							ImGui::Indent();
							switch (shapeType)
							{
							case en::PhysicShapeType::Circle:
							{
								b2CircleShape* circleShape = static_cast<b2CircleShape*>(shape);

								en::Vector2f position(circleShape->m_p.x, circleShape->m_p.y);
								if (en::ObjectEditor::ImGuiEditor(position, "Position"))
								{
									circleShape->m_p.Set(position.x, position.y);
									modified = true;
								}
								if (en::ObjectEditor::ImGuiEditor(circleShape->m_radius, "Radius"))
								{
									modified = true;
								}
							} break;
							case en::PhysicShapeType::Edge:
							{
								b2EdgeShape* edgeShape = static_cast<b2EdgeShape*>(shape);
								en::Vector2f v1(edgeShape->m_vertex1.x, edgeShape->m_vertex1.y);
								if (en::ObjectEditor::ImGuiEditor(v1, "v1"))
								{
									edgeShape->m_vertex1.Set(v1.x, v1.y);
									modified = true;
								}
								en::Vector2f v2(edgeShape->m_vertex2.x, edgeShape->m_vertex2.y);
								if (en::ObjectEditor::ImGuiEditor(v2, "v2"))
								{
									edgeShape->m_vertex2.Set(v2.x, v2.y);
									modified = true;
								}
								if (en::ObjectEditor::ImGuiEditor(edgeShape->m_hasVertex0, "HasV0"))
								{
									modified = true;
								}
								if (edgeShape->m_hasVertex0)
								{
									en::Vector2f v0(edgeShape->m_vertex0.x, edgeShape->m_vertex0.y);
									if (en::ObjectEditor::ImGuiEditor(v0, "v0"))
									{
										edgeShape->m_vertex0.Set(v0.x, v0.y);
										modified = true;
									}
								}
								if (en::ObjectEditor::ImGuiEditor(edgeShape->m_hasVertex3, "HasV3"))
								{
									modified = true;
								}
								if (edgeShape->m_hasVertex3)
								{
									en::Vector2f v3(edgeShape->m_vertex3.x, edgeShape->m_vertex3.y);
									if (en::ObjectEditor::ImGuiEditor(v3, "v3"))
									{
										edgeShape->m_vertex3.Set(v3.x, v3.y);
										modified = true;
									}
								}
							} break;
							case en::PhysicShapeType::Polygon:
							{
								b2PolygonShape* polygonShape = static_cast<b2PolygonShape*>(shape);
								bool polygonModified = false;
								b2Vec2 vertices[b2_maxPolygonVertices];
								en::U32 vertexCount = static_cast<en::U32>(polygonShape->m_count);
								for (en::U32 i = 0; i < vertexCount; ++i)
								{
									vertices[i] = polygonShape->m_vertices[i];
								}
								for (en::U32 i = 0; i < vertexCount; ++i)
								{
									en::Vector2f v(vertices[i].x, vertices[i].y);
									std::string vertexName = "v" + en::ToString(i);
									ImGui::PushID(vertexName.c_str());
									if (vertexCount > 3 && ImGui::Button("x"))
									{
										vertexCount--;
										for (en::U32 j = i; j < vertexCount; ++j)
										{ 
											vertices[j] = vertices[j + 1];
										}
										ImGui::SameLine();
										en::ObjectEditor::ImGuiEditor(v, vertexName.c_str()); // Only display
										polygonModified = true;
									}
									else
									{
										if (vertexCount > 3)
										{
											ImGui::SameLine();
										}
										if (en::ObjectEditor::ImGuiEditor(v, vertexName.c_str()))
										{
											vertices[i].Set(v.x, v.y);
											polygonModified = true;
										}
									}
									ImGui::PopID();
								}
								if (vertexCount < b2_maxPolygonVertices)
								{
									if (ImGui::Button("Add"))
									{
										vertices[vertexCount].Set(0.5f * (vertices[0].x + vertices[vertexCount - 1].x), 0.5f * (vertices[0].y + vertices[vertexCount - 1].y));
										vertexCount++;
										polygonModified = true;
									}
								}
								if (polygonModified)
								{
									polygonShape->Set(vertices, static_cast<int>(vertexCount));
									modified = true;
								}
							} break;
							case en::PhysicShapeType::Chain:
							{
								b2ChainShape* chainShape = static_cast<b2ChainShape*>(shape);
								
								// TODO : Modify chain vertices
								/*
								bool chainModified = false;
								std::vector<b2Vec2> vertices;
								en::U32 vertexCount = static_cast<en::U32>(chainShape->m_count);
								vertices.reserve(static_cast<std::size_t>(vertexCount));
								for (en::U32 i = 0; i < vertexCount; ++i)
								{
									vertices.push_back(chainShape->m_vertices[i]);
								}
								for (en::U32 i = 0; i < vertexCount; ++i)
								{
									en::Vector2f v(vertices[i].x, vertices[i].y);
									std::string vertexName = "v" + en::ToString(i);
									ImGui::PushID(vertexName.c_str());
									if (vertexCount > 2 && ImGui::Button("x"))
									{
										vertexCount--;
										for (en::U32 j = i; j < vertexCount; ++j)
										{
											vertices[j] = vertices[j + 1];
										}
										ImGui::SameLine();
										en::ObjectEditor::ImGuiEditor(v, vertexName.c_str()); // Only display
										chainModified = true;
									}
									else
									{
										if (vertexCount > 3)
										{
											ImGui::SameLine();
										}
										if (en::ObjectEditor::ImGuiEditor(v, vertexName.c_str()))
										{
											vertices[i].Set(v.x, v.y);
											chainModified = true;
										}
									}
									ImGui::PopID();
								}
								if (ImGui::Button("Add"))
								{
									vertices.push_back(b2Vec2(vertices[vertexCount - 1].x + 1.0f, vertices[vertexCount - 1].x + 1.0f));
									chainModified = true;
								}
								if (chainModified)
								{
									chainShape->CreateChain(vertices.data(), static_cast<int32>(vertices.size()));
								}
								*/

								if (en::ObjectEditor::ImGuiEditor(chainShape->m_hasPrevVertex, "HasPrevVertex"))
								{
									modified = true;
								}
								if (chainShape->m_hasPrevVertex)
								{
									en::Vector2f prev(chainShape->m_prevVertex.x, chainShape->m_prevVertex.y);
									if (en::ObjectEditor::ImGuiEditor(prev, "PrevVertex"))
									{
										chainShape->m_prevVertex.Set(prev.x, prev.y);
										modified = true;
									}
								}
								if (en::ObjectEditor::ImGuiEditor(chainShape->m_hasNextVertex, "HasNextVertex"))
								{
									modified = true;
								}
								if (chainShape->m_hasNextVertex)
								{
									en::Vector2f next(chainShape->m_nextVertex.x, chainShape->m_nextVertex.y);
									if (en::ObjectEditor::ImGuiEditor(next, "NextVertex"))
									{
										chainShape->m_nextVertex.Set(next.x, next.y);
										modified = true;
									}
								}
							} break;
							default: enAssert(false);
							}
							ImGui::Unindent();

							auto density = fixture->GetDensity();
							if (en::ObjectEditor::ImGuiEditor(density, "Density"))
							{
								fixture->SetDensity(density);
								body->ResetMassData(); // ResetMassData should be called
								modified = true;
							}
							auto friction = fixture->GetFriction();
							if (en::ObjectEditor::ImGuiEditor(friction, "Friction"))
							{
								fixture->SetFriction(friction);
								modified = true;
							}
							auto restitution = fixture->GetRestitution();
							if (en::ObjectEditor::ImGuiEditor(restitution, "Restitution"))
							{
								fixture->SetRestitution(restitution);
								modified = true;
							}
							auto sensor = fixture->IsSensor();
							if (en::ObjectEditor::ImGuiEditor(sensor, "Sensor"))
							{
								fixture->SetSensor(sensor);
								modified = true;
							}

							if (ImGui::CollapsingHeader("Filter"))
							{
								bool filterModified = false;
								b2Filter filter = fixture->GetFilterData();

								if (en::ObjectEditor::ImGuiEditor(filter.categoryBits, "Category"))
								{
									filterModified = true;
								}
								if (en::ObjectEditor::ImGuiEditor(filter.maskBits, "Mask"))
								{
									filterModified = true;
								}
								if (en::ObjectEditor::ImGuiEditor(filter.groupIndex, "GroupIndex"))
								{
									filterModified = true;
								}

								if (filterModified)
								{
									fixture->SetFilterData(filter);
								}
							}

							if (ImGui::Button("Remove"))
							{
								b2Fixture* temp = fixture;
								fixture = fixture->GetNext();
								body->DestroyFixture(temp);
							}
							else
							{
								fixture = fixture->GetNext();
							}
						}

						ImGui::PopID();
						fixtureID++;
					}
				}
				ImGui::Unindent();
			}
			ImGui::Unindent();
		}
		return modified;
	}
};

template <>
struct CustomComponentInitialization<en::PhysicComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, en::PhysicComponent& component)
	{
		if (entity.IsValid())
		{
			en::World& world = const_cast<en::World&>(entity.GetWorld());
			if (world.HasPhysicSystem())
			{
				return world.GetPhysicSystem()->Initialize(entity, component);
			}
		}
		return false;
	}
};