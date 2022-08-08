// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolsFunctionLibrary.generated.h"

/**
 * Tools blueprint function library
 */
UCLASS()
class PMUEINTEGRATION_API UToolsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static int64 NumberOfAtoms(FString Filename, bool OnlyAtom = true);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static TArray<FString> AcquireSymbols(FString Filename, bool OnlyAtom = true);

	UFUNCTION(BlueprintCallable, Category = "File I/O")	
	static TSet<FString> UniqueElements(FString Filename);

	UFUNCTION(BlueprintCallable, Category="Return useful constants")
	FVector GetElementColourRGB(FString AtomSymbol);

	UFUNCTION(BlueprintCallable, Category = "Return useful constants")
	double GetVDWRadius(FString AtomSymbol);




};