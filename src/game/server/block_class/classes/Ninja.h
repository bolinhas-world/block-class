/* Block Class System - Ninja Class */
#ifndef GAME_SERVER_BLOCK_CLASS_NINJA_H
#define GAME_SERVER_BLOCK_CLASS_NINJA_H

#include "IPlayerClass.h"

/*
	NINJA CLASS
	
	Características:
	- Classe ágil e rápida
	- Velocidade aumentada
	- Gancho mais longo
	- Vida e armadura reduzidas
	- Habilidade especial: Dash rápido que teleporta curta distância
*/

class CNinjaClass : public IPlayerClass
{
public:
	CNinjaClass() : IPlayerClass(), m_DashCooldownEnd(0) {}
	~CNinjaClass() override {}

	// Informações da classe
	EPlayerClass GetClassType() const override { return CLASS_NINJA; }
	const char* GetClassName() const override { return "Ninja"; }
	const char* GetDescription() const override 
	{ 
		return "Fast and agile class. High speed, longer hook. Lower health."; 
	}

	// Lifecycle hooks
	void OnSpawn(CCharacter* pCharacter) override;
	void OnDeath(CCharacter* pCharacter, int Killer, int Weapon) override;
	void OnTick(CCharacter* pCharacter) override;
	void OnWeaponFire(CCharacter* pCharacter, int Weapon) override;
	void OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon) override;

	// Stats base da classe
	int GetMaxHealth() const override { return 7; }  // -3 do padrão
	int GetMaxArmor() const override { return 7; }   // -3 do padrão
	float GetSpeedMultiplier() const override { return 1.35f; }  // 35% mais rápido
	float GetDamageMultiplier() const override { return 1.0f; }  // Dano normal
	int GetHookLengthBonus() const override { return 5; }  // +5 tiles no gancho

	// Habilidades especiais
	bool HasDoubleJump() const override { return true; }
	bool HasDash() const override { return true; }
	int GetSpecialCooldown() const override { return 50 * 8; }  // 8 segundos
	void UseSpecialAbility(CCharacter* pCharacter) override;

private:
	int m_DashCooldownEnd;
	int m_JumpsUsed;
	bool m_WasGrounded;
	
	static const int DASH_DISTANCE = 15;  // Distância do dash em tiles
};

#endif // GAME_SERVER_BLOCK_CLASS_NINJA_H
