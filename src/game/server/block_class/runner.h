#ifndef GAME_SERVER_BLOCK_CLASS_RUNNER_H
#define GAME_SERVER_BLOCK_CLASS_RUNNER_H

#include "block_class.h"

class CBlockClassRunner final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	void OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter) override;

private:
	float m_GroundSpeedMultiplier = 1.5f;
	float m_GroundAccelMultiplier = 1.5f;
};

#endif
