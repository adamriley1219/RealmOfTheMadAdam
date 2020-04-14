#include "Game/Systems/CombatSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/AbilityComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/TriggerComp.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

#include "Engine/Memory/Debug/LogProfile.hpp"

//--------------------------------------------------------------------------
/**
* CombatSystem
*/
CombatSystem::CombatSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~CombatSystem
*/
CombatSystem::~CombatSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void CombatSystem::Update( float delta_time ) const
{
	UNUSED(delta_time);
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{
		if( !entity.m_claimed )
		{
			continue;
		}

		// Check if its time for me to die
		IntentComp* intent = (IntentComp*) entity.GetComponent( INTENT_COMP );
		if (intent)
		{
			if (intent->m_death_timer.HasElapsed())
			{
				entity.m_destroy = true;
				continue;
			}
		}

		// Check if I die based on health
		StatsComp* stats_comp = (StatsComp*) entity.GetComponent( STATS_COMP );
		if (stats_comp)
		{
			if (stats_comp->m_health <= 0.0f)
			{
				entity.m_destroy = true;
				continue;
			}
		}

		// Check if we should fire
		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		if( intent && trans_comp && stats_comp )
		{
			if( !intent->m_fire_timer.HasElapsed() )
			{
				continue;
			}

			if( intent->m_wants_to_fire )
			{
				// Spawn bullet
				Fire( "not_used_yet", intent->m_desired_fire_direction, stats_comp->m_damage_multiplier, trans_comp->m_transform, stats_comp->m_team );
				
				intent->m_fire_timer.Reset();
			}
			
			if( intent->m_wants_to_fire_secondary )
			{
				intent->m_fire_timer.Reset();
			}
		}
	}
}

//--------------------------------------------------------------------------
/**
* Fire
*/
void CombatSystem::Fire( const std::string& ability_name, const Vec2& direction, float damage_multiplier, const Transform2D& starting_treansform, eTeam team ) const
{
	UNUSED( ability_name );
	Entity* ability = GetCurrentAdmin().CreateEntity();
	IntentComp* ability_intent = (IntentComp*)ability->AddComponent(INTENT_COMP);
	PhysicsComp* ability_physics = (PhysicsComp*)ability->AddComponent(PHYSICS_COMP);
	TriggerComp* ability_trigger = (TriggerComp*)ability->AddComponent(TRIGGER_COMP);
	TransformComp* ability_transform = (TransformComp*)ability->AddComponent(TRANSFORM_COMP);
	AbilityComp* ability_comp = (AbilityComp*)ability->AddComponent(ABILITY_COMP);
	RenderComp* ability_render = (RenderComp*)ability->AddComponent(RENDER_COMP);
	((StatsComp*)ability->AddComponent(STATS_COMP))->m_team = team;
	ability_intent->m_desired_move_direction = direction;
	ability_intent->m_death_timer.SetAndReset(0.75f);
	UNUSED(ability_comp);

	// Physics
	ability_physics->m_radius = 0.15f;
	ability_physics->m_static_object = false;
	ability_physics->m_max_speed = 4.0f;
	ability_physics->m_acceleration = 100.0f;

	// setup main graphic
	SpriteSheet sheet( (TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile("Data/Images/Extras_4x4.png"), IntVec2(4, 4) );
	const SpriteDefinition& def = sheet.GetSpriteDefinition(0, 3);
	Vec2 bot_left_uvs;
	Vec2 top_right_uvs;
	def.GetUVs(bot_left_uvs, top_right_uvs);

	AddVertsForAABB2D(ability_render->m_main_group.verts, AABB2(ability_physics->m_radius * 2.0f, ability_physics->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	ability_render->m_main_texture = "Data/Images/Extras_4x4.png";

	// Trigger setup
	ability_trigger->m_die_on_contect = true;
	ability_trigger->m_damage_on_triggered = 1.0f * damage_multiplier;

	ability_transform->m_transform = starting_treansform;

}
