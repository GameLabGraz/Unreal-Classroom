#include "PickupActor.h"
#include "Components/SphereComponent.h"

#include "HammeringStatics.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Grabbable);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Grabbable,ECR_Block);
	StaticMeshComponent->SetSimulatePhysics(true);

	CustomAttachPoint = CreateDefaultSubobject<USphereComponent>(TEXT("CustomAttachPoint"));
	CustomAttachPoint->SetupAttachment(StaticMeshComponent);
	CustomAttachPoint->SetSphereRadius(10.0f);
	CustomAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
	CustomAttachPoint->ShapeColor = FColor::Emerald;
	CustomAttachPoint->SetHiddenInGame(true);
	
}

void APickupActor::GrabPressed(USceneComponent* AttachTo)
{
	StaticMeshComponent->SetSimulatePhysics(false);

	if(bSnapToHand)
	{
		StaticMeshComponent->AttachToComponent(AttachTo,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		StaticMeshComponent->AttachToComponent(AttachTo,FAttachmentTransformRules::KeepWorldTransform);
	}
}

void APickupActor::GrabReleased()
{
	StaticMeshComponent->SetSimulatePhysics(true);	
}

int APickupActor::GetGrabType()
{
	return TypeOfGrab;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	if(bIsShowingDebug)
	{
		CustomAttachPoint->SetHiddenInGame(false);
	}
}

FVector APickupActor::GetCustomAttachLocation() const
{
	return CustomAttachPoint->GetComponentLocation();
}

FRotator APickupActor::GetCustomAttachRotation() const
{
	return CustomAttachPoint->GetComponentRotation();
}

void APickupActor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

