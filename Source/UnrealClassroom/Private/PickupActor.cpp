#include "PickupActor.h"

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
}

void APickupActor::GrabPressed(USceneComponent* AttachTo)
{
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->AttachToComponent(AttachTo,FAttachmentTransformRules::KeepWorldTransform);
}

void APickupActor::GrabReleased()
{
	StaticMeshComponent->SetSimulatePhysics(true);	
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

