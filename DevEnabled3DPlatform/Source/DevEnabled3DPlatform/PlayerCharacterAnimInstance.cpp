#include "PlayerCharacterAnimInstance.h"

#include "PlayerCharacter.h"

void UPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::UpdateParameters()
{
	if (PlayerCharacter == nullptr) return;

	MoveSpeed = PlayerCharacter->GetVelocity().Length();
	IsJumping = PlayerCharacter->GetIsJumping();
	IsInAir = PlayerCharacter->GetIsInAir();
}