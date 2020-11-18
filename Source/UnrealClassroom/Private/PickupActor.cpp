#include "PickupActor.h"

#include "UnrealClassRoomStatics.h"
#include "Components/SphereComponent.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Grabbable);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetSimulatePhysics(true);

	CustomAttachPoint = CreateDefaultSubobject<USphereComponent>(TEXT("CustomAttachPoint"));
	CustomAttachPoint->SetupAttachment(StaticMeshComponent);
	CustomAttachPoint->SetSphereRadius(10.0f);
	CustomAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
	CustomAttachPoint->ShapeColor = FColor::Emerald;
	CustomAttachPoint->SetHiddenInGame(true);

}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupActor::GrabPressed(USceneComponent* AttachTo)
{
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if(bSnapHandToMesh)
	{
		AttachTo->SetWorldLocation(CustomAttachPoint->GetComponentLocation());
		StaticMeshComponent->AttachToComponent(AttachTo,FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		StaticMeshComponent->AttachToComponent(AttachTo,FAttachmentTransformRules::KeepWorldTransform);
	}
}

void APickupActor::GrabReleased()
{
	StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetSimulatePhysics(true);
}

int APickupActor::GetGrabType()
{
	return TypeOfGrab;
}

