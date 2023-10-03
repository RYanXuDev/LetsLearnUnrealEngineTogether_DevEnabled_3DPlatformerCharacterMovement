#include "PlayerCharacterAnimInstance.h"

#include "PlayerCharacter.h"

void UPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateParameters();
}

void UPlayerCharacterAnimInstance::UpdateParameters()
{
	if (PlayerCharacter == nullptr) return;

	MoveSpeed = PlayerCharacter->GetVelocity().Length();
	IsJumping = PlayerCharacter->GetIsJumping();
	IsAirJumping = PlayerCharacter->GetIsAirJumping();
	IsFalling = PlayerCharacter->GetIsFalling();
	IsWallSliding = PlayerCharacter->GetIsWallSliding();
	IsWallJumping = PlayerCharacter->GetIsWallJumping();
	IsGrounded = PlayerCharacter->GetIsGrounded();
	IsSliding = PlayerCharacter->GetIsSliding();
}