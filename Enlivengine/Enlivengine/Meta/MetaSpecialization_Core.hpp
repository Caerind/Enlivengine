#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Meta/MetaSpecialization_Graphics.hpp>
#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/Engine.hpp>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/PhysicComponent.hpp>
#include <Enlivengine/Core/CameraComponent.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

//////////////////////////////////////////////////////////////////
// en::NameComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::NameComponent)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::UIDComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::UIDComponent)
	ENLIVE_META_CLASS_MEMBER("id", &en::UIDComponent::id)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::RenderableComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::RenderableComponent)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::TransformComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::TransformComponent)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::TransformComponent>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::TransformComponent& object, const char* name)
	{
		bool modified = false;
		if (en::ObjectEditor::ImGuiEditor(static_cast<en::Transform&>(object), name))
		{
			object.MarkGlobalMatrixAsDirty();
			modified = true;
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::SpriteComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::SpriteComponent)
	ENLIVE_META_CLASS_MEMBER("sprite", &en::SpriteComponent::sprite)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::TilemapComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::TilemapComponent)
	ENLIVE_META_CLASS_MEMBER("tilemap", &en::TilemapComponent::tilemap)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::CameraComponent
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::CameraComponent)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::CameraComponent>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::CameraComponent& object, const char* name)
	{
		bool modified = false;
#ifdef ENLIVE_DEBUG
		if (en::World* world = en::Engine::GetCurrentWorld())
		{
			world->GetDebugDraw().DrawFrustum(object.CreateFrustum());
		}
#endif // ENLIVE_DEBUG
		if (en::ObjectEditor::ImGuiEditor(static_cast<en::Camera&>(object), name))
		{
			modified = true;
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::Entity
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Entity)
ENLIVE_META_CLASS_END()

template <>
struct HasCustomSerialization<en::Entity>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::Entity>();
			const auto& componentInfos = en::ComponentManager::GetComponentInfos();
			const auto endItr = componentInfos.cend();
			for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
			{
				const auto& ci = itr->second;
				if (HasComponent(object, ci.enttID))
				{
					ci.serialize(dataFile, object);
				}
			}
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::Entity& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (strcmp(name, "") == 0)
		{
			bool anyError = false;

			// We should already be on the node of the entity : See explanation on EntityManager::Deserialize
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Entity>::GetHash());
			if (parser.ReadFirstNode())
			{
				static std::vector<DeserializationComponentNode> componentNodes; // TODO : Move to Array
				componentNodes.clear();
				do
				{
					const std::string nodeName = parser.GetNodeName();
					const en::U32 nodeNameHash = en::Hash::SlowHash(nodeName);
					const en::U32 nodeType = dataFile.ReadCurrentType();
					const bool registeredComponent = en::ComponentManager::IsRegistered(nodeNameHash);
					if (nodeNameHash == nodeType && registeredComponent)
					{
						componentNodes.push_back({ nodeName, nodeNameHash });
					}
					else
					{
						if (nodeNameHash != nodeType)
						{
							enLogWarning(en::LogChannel::Core, "Incompatible component : {}({}) <-> {} ?", nodeNameHash, nodeName, nodeType);
						}
						if (!registeredComponent)
						{
							enLogWarning(en::LogChannel::Core, "Unregistered component : {}", nodeName);
						}
						anyError = true;
					}
				} while (parser.NextSibling());
				parser.CloseNode();

				// Now, we are back at the entity level, parse components
				for (const auto& componentNode : componentNodes)
				{
					const auto& componentInfos = en::ComponentManager::GetComponentInfos();
					enAssert(componentInfos.find(componentNode.hash) != componentInfos.end());
					if (!componentInfos.at(componentNode.hash).deserialize(dataFile, object))
					{
						anyError = true;
					}
				}
			}
			return !anyError;
		}
		else
		{
			// Single entity deserialization is not supported yet
			enAssert(false);
			return false;
		}
	}

private:
	struct DeserializationComponentNode
	{
		std::string name;
		en::U32 hash;
	};

	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomObjectEditor<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::Entity>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Entity& object, const char* name)
	{
		bool display = true;
		bool onlyThisOneSelected = object.GetWorld().GetSelectedEntities().size() == 1;
		if (!onlyThisOneSelected)
		{
			display = ImGui::CollapsingHeader(name);
		}

		bool modified = false;
		if (display)
		{
			if (!onlyThisOneSelected)
			{
				ImGui::Indent();
			}
			if (object.IsValid())
			{
				const en::U32 entityID = object.GetID();
				ImGui::Text("%s (ID: %d)", name, entityID);

				ImGui::SameLine();
				bool destroyed = false;
				if (ImGui::SmallButton(ICON_FA_BAN))
				{
					destroyed = true;
				}

				ImGui::PushID(entityID);

				const auto& componentInfos = en::ComponentManager::GetComponentInfos();
				static std::vector<en::U32> hasNot;
				hasNot.clear();
				const auto endItr = componentInfos.cend();
				for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
				{
					const auto& ci = itr->second;
					if (HasComponent(object, ci.enttID))
					{
						ImGui::PushID(ci.enttID);
						if (ImGui::Button("-"))
						{
							ci.remove(object);
							modified = true;
							ImGui::PopID();
							continue;
						}
						else
						{
							ImGui::SameLine();
							if (ci.editor(object))
							{
								modified = true;
							}
							ImGui::PopID();
						}
					}
					else
					{
						hasNot.push_back(itr->first);
					}
				}

				if (!hasNot.empty())
				{
					if (ImGui::Button("+ Add Component"))
					{
						ImGui::OpenPopup("Add Component");
					}

					if (ImGui::BeginPopup("Add Component"))
					{
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();
						for (auto componentHash : hasNot)
						{
							const auto& ci = componentInfos.at(componentHash);
							ImGui::PushID(componentHash);
							if (ImGui::Selectable(ci.name))
							{
								ci.add(object);
								modified = true;
							}
							ImGui::PopID();
						}
						ImGui::EndPopup();
					}
				}

				ImGui::PopID();

				if (destroyed)
				{
					object.Destroy();
				}
			}
			else
			{
				ImGui::Text("Invalid entity");
			}
			if (!onlyThisOneSelected)
			{
				ImGui::Unindent();
			}
		}
		return modified;
	}

private:
	using ComponentTypeID = ENTT_ID_TYPE;
	static bool HasComponent(const en::Entity& entity, ComponentTypeID enttComponentID)
	{
		// TODO : Factorize with CustomXmlSerialization<en::Entity>
		const ComponentTypeID type[] = { enttComponentID };
		return entity.GetRegistry().runtime_view(std::cbegin(type), std::cend(type)).contains(entity.GetEntity());
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::EntityManager
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::EntityManager)
ENLIVE_META_CLASS_END()

template <>
struct HasCustomSerialization<en::EntityManager>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::EntityManager& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			bool anyError = false;
			dataFile.WriteCurrentType<en::EntityManager>();
			object.Each([&dataFile, &object, &anyError](auto entityEntt)
				{
					en::Entity entity(const_cast<en::EntityManager&>(object), entityEntt);
					if (entity.IsValid())
					{
						if (!HasCustomSerialization<en::Entity>::Serialize(dataFile, entity, entity.GetName()))
						{
							anyError = true;
						}
					}
				});
			parser.CloseNode();
			return !anyError;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::EntityManager& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			bool anyError = false;
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::EntityManager>::GetHash());
			if (parser.ReadFirstNode())
			{
				do
				{
					en::Entity entity = object.CreateEntity();
					if (entity.IsValid())
					{
						static const char* invalidEntityName = ""; // We already have the node open + we can have many entities with the same name
						if (!HasCustomSerialization<en::Entity>::Deserialize(dataFile, entity, invalidEntityName))
						{
							anyError = true;
						}
					}
				} while (parser.NextSibling());
				parser.CloseNode();
			}
			parser.CloseNode();
			return !anyError;
		}
		else
		{
			return false;
		}
	}
};

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::EntityManager>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EntityManager& object, const char* name)
	{
		enAssert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			if (ImGui::Button("New Entity"))
			{
				object.CreateEntity();
				modified = true;
			}
			object.Each([&object, &modified](auto entityEntt)
				{
					en::Entity entity(object, entityEntt);
					if (entity.IsValid())
					{
						ImGui::PushID(entity.GetID());
						if (ImGui::Button("x"))
						{
							object.DestroyEntity(entity);
							modified = true;
						}
						else
						{
							ImGui::SameLine();
							if (HasCustomEditor<en::Entity>::ImGuiEditor(entity, entity.GetName()))
							{
								modified = true;
							}
						}
						ImGui::PopID();
					}
				});
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::PhysicComponent
//////////////////////////////////////////////////////////////////
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

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::PhysicComponent>
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

						en::PhysicShapeType shapeType = en::FromB2ShapeType(shape->GetType());
						if (en::ObjectEditor::ImGuiEditor(shapeType, "ShapeType"))
						{
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
#endif // ENLIVE_ENABLE_IMGUI
