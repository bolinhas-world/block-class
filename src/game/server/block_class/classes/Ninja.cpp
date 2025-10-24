/* Block Class System - Ninja Class Implementation */
#include "Ninja.h"
#include <game/server/entities/character.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>

void CNinjaClass::OnSpawn(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	// Reset dash cooldown
	m_DashCooldownEnd = 0;
	m_JumpsUsed = 0;
	m_WasGrounded = true;

	// Aplicar stats da classe (vida e armadura reduzidas)
	// O character já spawna com 10/10, então reduzimos para 7/7
	// Isso será feito ajustando os max values

	// Mensagem de spawn
	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(pPlayer)
	{
		pCharacter->GameServer()->SendBroadcast("Ninja Class - High Speed, Long Hook, Low HP", pPlayer->GetCid());
	}
}

void CNinjaClass::OnDeath(CCharacter* pCharacter, int Killer, int Weapon)
{
	// Reset dash cooldown ao morrer
	m_DashCooldownEnd = 0;
	m_JumpsUsed = 0;
}

void CNinjaClass::OnTick(CCharacter* pCharacter)
{
	if(!pCharacter)
		return;

	// Gerenciar double jump
	bool IsGrounded = pCharacter->IsGrounded();
	
	if(IsGrounded)
	{
		// Reset jumps quando tocar o chão
		m_JumpsUsed = 0;
	}
	
	m_WasGrounded = IsGrounded;

	// Mostrar cooldown do dash se necessário
	int CurrentTick = pCharacter->GameServer()->Server()->Tick();
	if(m_DashCooldownEnd > CurrentTick)
	{
		int TicksLeft = m_DashCooldownEnd - CurrentTick;
		int SecondsLeft = TicksLeft / 50;
		
		if(TicksLeft % 50 == 0)  // Atualizar a cada segundo
		{
			CPlayer* pPlayer = pCharacter->GetPlayer();
			if(pPlayer)
			{
				char aBuf[128];
				str_format(aBuf, sizeof(aBuf), "Dash ready in: %d seconds", SecondsLeft);
				pCharacter->GameServer()->SendBroadcast(aBuf, pPlayer->GetCid());
			}
		}
	}
}

void CNinjaClass::OnWeaponFire(CCharacter* pCharacter, int Weapon)
{
	// Ninja não tem modificações especiais no tiro
}

void CNinjaClass::OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon)
{
	// Ninja recebe dano normal
}

void CNinjaClass::UseSpecialAbility(CCharacter* pCharacter)
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
			str_format(aBuf, sizeof(aBuf), "Dash cooldown: %d seconds", SecondsLeft);
			pCharacter->GameServer()->SendBroadcast(aBuf, pPlayer->GetCid());
		}
		return;
	}

	// Executar dash
	// Calcular direção baseada no input do jogador
	vec2 Direction = vec2(0, 0);
	
	// Pegar input do player
	CPlayer* pPlayer = pCharacter->GetPlayer();
	if(!pPlayer)
		return;

	// Usar a direção que o personagem está olhando
	// Se não houver input de movimento, usar a direção do mouse
	int InputDirection = 0;  // Seria necessário acessar o input real
	
	// Por simplicidade, usar a direção atual do core
	float Angle = pCharacter->Core()->m_Angle;
	Direction.x = cosf(Angle);
	Direction.y = sinf(Angle);
	
	// Aplicar dash (teleporte na direção)
	vec2 DashVector = normalize(Direction) * (DASH_DISTANCE * 32.0f);  // 32 pixels por tile
	vec2 NewPos = pCharacter->GetPos() + DashVector;
	
	// Verificar colisão antes de teleportar
	// Por enquanto, teleportar diretamente
	pCharacter->SetPosition(NewPos);
	
	// Marcar cooldown
	SetSpecialUsed(CurrentTick);
	m_DashCooldownEnd = CurrentTick + GetSpecialCooldown();

	// Feedback visual
	pCharacter->SetEmote(EMOTE_SURPRISE, CurrentTick + 50);
	pCharacter->GameServer()->SendBroadcast("DASH!", pPlayer->GetCid());

	// Criar efeito visual (opcional - poderia criar partículas)
}
