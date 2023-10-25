// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LWRCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LIVEWIRERECHARGED_API ULWRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	ULWRCharacterMovementComponent();

	FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

protected:
	void UpdateFromCompressedFlags(const uint8 flags) override;
	void OnMovementUpdated(const float deltaTime, const FVector& oldLocation, const FVector& oldVelocity) override;

private:

	class FSavedMove_LWR : public FSavedMove_Character
	{
		using Super = FSavedMove_Character;

	private:
		bool CanCombineWith(const FSavedMovePtr& newMove, ACharacter* inCharacter, float maxDelta) const override;
		void Clear() override;
		uint8 GetCompressedFlags() const override;
		void SetMoveFor(ACharacter* c, float deltaTime, FVector const& newAccel, FNetworkPredictionData_Client_Character& clientData) override;
		void PrepMoveFor(ACharacter* c) override;

		uint8 saved_wantsToSprint : 1; // Bitfield for compression
	};

	class FNetworkPredictionData_Client_LWR : public FNetworkPredictionData_Client_Character
	{
	public:
		using Super = FNetworkPredictionData_Client_Character;

		FNetworkPredictionData_Client_LWR(const UCharacterMovementComponent& clientMovement);
		FSavedMovePtr AllocateNewMove() override;
	};


	bool safe_wantsToSprint = false;
};
