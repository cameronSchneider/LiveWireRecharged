// Fill out your copyright notice in the Description page of Project Settings.


#include "LWRCharacterMovementComponent.h"
#include "GameFramework/Character.h"

bool ULWRCharacterMovementComponent::FSavedMove_LWR::CanCombineWith(const FSavedMovePtr& newMove, ACharacter* inCharacter, float maxDelta) const
{
	const FSavedMove_LWR* newLWRMove = static_cast<FSavedMove_LWR*>(newMove.Get());
	bool result = true;

	result = result && saved_wantsToSprint == newLWRMove->saved_wantsToSprint;
	result = result && Super::CanCombineWith(newMove, inCharacter, maxDelta);;

	return result;
}

void ULWRCharacterMovementComponent::FSavedMove_LWR::Clear()
{
	saved_wantsToSprint = 0;
	Super::Clear();
}

uint8 ULWRCharacterMovementComponent::FSavedMove_LWR::GetCompressedFlags() const
{
	// This is how clients replicate essential movement data efficiently
	uint8 result = Super::GetCompressedFlags();

	if (saved_wantsToSprint)
	{
		result |= FLAG_Custom_0;
	}

	return result;
}

void ULWRCharacterMovementComponent::FSavedMove_LWR::SetMoveFor(ACharacter* c, float deltaTime, FVector const& newAccel, FNetworkPredictionData_Client_Character& clientData)
{
	Super::SetMoveFor(c, deltaTime, newAccel, clientData);

	ULWRCharacterMovementComponent* component = Cast<ULWRCharacterMovementComponent>(c->GetCharacterMovement());
	saved_wantsToSprint = component->safe_wantsToSprint;
}

void ULWRCharacterMovementComponent::FSavedMove_LWR::PrepMoveFor(ACharacter* c)
{
	Super::PrepMoveFor(c);

	ULWRCharacterMovementComponent* component = Cast<ULWRCharacterMovementComponent>(c->GetCharacterMovement());
	component->safe_wantsToSprint = saved_wantsToSprint;
}

ULWRCharacterMovementComponent::FNetworkPredictionData_Client_LWR::FNetworkPredictionData_Client_LWR(const UCharacterMovementComponent& clientMovement)
	: Super(clientMovement)
{
}

FSavedMovePtr ULWRCharacterMovementComponent::FNetworkPredictionData_Client_LWR::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_LWR());
}

ULWRCharacterMovementComponent::ULWRCharacterMovementComponent()
{
}

FNetworkPredictionData_Client* ULWRCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		ULWRCharacterMovementComponent* mutableComponent = const_cast<ULWRCharacterMovementComponent*>(this); // Yuck, but we need to modify this in a const function
		mutableComponent->ClientPredictionData = new FNetworkPredictionData_Client_LWR(*this);
		mutableComponent->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		mutableComponent->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void ULWRCharacterMovementComponent::UpdateFromCompressedFlags(const uint8 flags)
{
	Super::UpdateFromCompressedFlags(flags);

	safe_wantsToSprint = (flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void ULWRCharacterMovementComponent::OnMovementUpdated(const float deltaTime, const FVector& oldLocation, const FVector& oldVelocity)
{
	Super::OnMovementUpdated(deltaTime, oldLocation, oldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (safe_wantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}


void ULWRCharacterMovementComponent::SprintPressed()
{
	safe_wantsToSprint = true;
}

void ULWRCharacterMovementComponent::SprintReleased()
{
	safe_wantsToSprint = false;
}