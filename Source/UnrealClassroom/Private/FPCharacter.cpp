#include "FPCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "HandSkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"

AFPCharacter::AFPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Default Capsule collider size
	GetCapsuleComponent()->InitCapsuleSize(20.f, 96.0f);

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetCapsuleComponent());
	FPCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position of Camera based of FPS camera
	FPCamera->bUsePawnControlRotation = true;

	RightHandComponent = CreateDefaultSubobject<UHandSkeletalMeshComponent>(TEXT("RightHand"));
	RightHandComponent->SetupAttachment(FPCamera);
	RightHandComponent->SetRelativeLocation(FVector(60.0f, 20.0f, -12.0f));
	RightHandComponent->GrabSphereComponent->SetRelativeLocation(FVector(6,2,0));

	LeftHandComponent = CreateDefaultSubobject<UHandSkeletalMeshComponent>(TEXT("LeftHand"));
	LeftHandComponent->SetupAttachment(FPCamera);
	LeftHandComponent->SetRelativeLocation(FVector(60.0f, -20.0f, -12.0f));
	LeftHandComponent->GrabSphereComponent->SetRelativeLocation(FVector(6,-2,0));

}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialRelativeLocationRightHand = RightHandComponent->GetRelativeLocation();
	InitialRelativeLocationLeftHand = LeftHandComponent->GetRelativeLocation();
	
}

void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GrabRaycast();

}

void AFPCharacter::GrabRaycast()
{
	auto const StartLocation = FPCamera->GetComponentLocation();
	auto const EndLocation = StartLocation + FPCamera->GetForwardVector() * (MaxGrabDistance + HandDimension);
	IsGrabRaycastHit = GetWorld()->LineTraceSingleByChannel(GrabRaycastResult, StartLocation, EndLocation,ECC_Visibility);

	if(IsGrabRaycastHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Move Forward: %s "), *GrabRaycastResult.GetActor()->GetName()));
	}
}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAtRate", this, &AFPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookupAtRate", this, &AFPCharacter::LookupAtRate);

	PlayerInputComponent->BindAxis("GrabRight", this, &AFPCharacter::GrabRight);
	PlayerInputComponent->BindAxis("GrabLeft", this, &AFPCharacter::GrabLeft);
}

void AFPCharacter::MoveForward(float Value)
{
	if(IsDebuggingController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Move Forward: %f "), Value));
	}

	if(Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPCharacter::MoveRight(float Value)
{
	if(IsDebuggingController)
	{
		
	}

	if(Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * TurnBaseRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::LookupAtRate(float Value)
{
	AddControllerPitchInput(Value * LookupBaseRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::GrabRight(float Value)
{
	auto GrabRange = MaxGrabDistance;
	if(IsGrabRaycastHit)
	{
		GrabRange = FVector::Distance(FPCamera->GetComponentLocation(), GrabRaycastResult.Location) - HandDimension;
	}
	
	Value = FMath::Clamp(Value,0.0f,1.0f);
	const auto NewLocation = FMath::Lerp(InitialRelativeLocationRightHand, FVector(1,0,0) * GrabRange, Value);

	RightHandComponent->SetRelativeLocation(NewLocation);
}

void AFPCharacter::GrabLeft(float Value)
{
	auto GrabRange = MaxGrabDistance;
	if(IsGrabRaycastHit)
	{
		GrabRange = FVector::Distance(FPCamera->GetComponentLocation(), GrabRaycastResult.Location) - HandDimension;
	}
	
	Value = FMath::Clamp(Value,0.0f,1.0f);
	const auto NewLocation = FMath::Lerp(InitialRelativeLocationLeftHand, FVector(1,0,0) * GrabRange, Value);

	LeftHandComponent->SetRelativeLocation(NewLocation);
}

