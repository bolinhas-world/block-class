/* Block Class System - Soldier Class Implementation */
#include "Soldier.h"
#include <game/server/entities/character.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>

void CSoldierClass::OnSpawn(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	// Reset shield state
	m_ShieldActive = false;
	m_ShieldEndTick = 0;

	// Aplicar stats da classe
	pCharacter->IncreaseHealth(GetMaxHealth() - 10);  // Ajustar para o máximo da classe
	pCharacter->IncreaseArmor(GetMaxArmor() - 10);

	// Mensagem de spawn
	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(pPlayer)
	{
		pCharacter->GameServer()->SendBroadcast("Soldier Class - High HP/Armor, Slower Movement", pPlayer->GetCid());
	}
}

void CSoldierClass::OnDeath(CCharacter* pCharacter, int Killer, int Weapon)
{
	// Desativar shield ao morrer
	m_ShieldActive = false;
	m_ShieldEndTick = 0;
}

void CSoldierClass::OnTick(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	// Verificar se shield expirou
	if(m_ShieldActive && pCharacter->GameServer()->Server()->Tick() >= m_ShieldEndTick)
	{
		m_ShieldActive = false;
		
		CPlayer* pPlayer = pCharacter->GetPlayer();
		if(pPlayer)
		{
			pCharacter->GameServer()->SendBroadcast("Shield deactivated!", pPlayer->GetCid());
		}
	}

	// Mostrar status do shield se estiver ativo
	if(m_ShieldActive)
	{
		CPlayer* pPlayer = pCharacter->GetPlayer();
		if(pPlayer)
		{
			int TicksLeft = m_ShieldEndTick - pCharacter->GameServer()->Server()->Tick();
			int SecondsLeft = TicksLeft / 50;
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "SHIELD ACTIVE: %d seconds", SecondsLeft);
			pCharacter->GameServer()->SendBroadcast(aBuf, pPlayer->GetCid());
		}
	}
}

void CSoldierClass::OnWeaponFire(CCharacter* pCharacter, int Weapon)
{
	// Soldier não tem modificações especiais no tiro
	// O multiplicador de dano é aplicado no OnPlayerDamage
}

void CSoldierClass::OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon)
{
	// Se o shield estiver ativo, reduzir dano recebido
	if(m_ShieldActive)
	{
		// O dano já foi aplicado, essa função é apenas informativa
		// A redução real seria feita no TakeDamage do character
	}
}

void CSoldierClass::UseSpecialAbility(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	int CurrentTick = pCharacter->GameServer()->Server()->Tick();
	
	if(!CanUseSpecial(CurrentTick))
	{
		// Ainda em cooldown
		int TicksLeft = GetSpecialCooldown() - (CurrentTick - m_LastSpecialUse);
		int SecondsLeft = (TicksLeft / 50) + 1;
		
		CPlayer* pPlayer = pCharacter->GetPlayer();
		if(pPlayer)
		{
			char aBuf[128];
			str_format(aBuf, sizeof(aBuf), "Shield cooldown: %d seconds", SecondsLeft);
			pCharacter->GameServer()->SendBroadcast(aBuf, pPlayer->GetCid());
		}
		return;
	}

	// Ativar shield
	m_ShieldActive = true;
	m_ShieldEndTick = CurrentTick + SHIELD_DURATION;
	SetSpecialUsed(CurrentTick);

	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(pPlayer)
	{
		pCharacter->GameServer()->SendBroadcast("SHIELD ACTIVATED! 75% damage reduction for 5 seconds", pPlayer->GetCid());
	}

	// Efeito visual (emote)
	pCharacter->SetEmote(EMOTE_HAPPY, CurrentTick + 50 * 2);
}
