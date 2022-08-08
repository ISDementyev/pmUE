// See GPL-v3.0 LICENSE in GitHub repo.


#include "ToolsFunctionLibrary.h"

int64 UToolsFunctionLibrary::NumberOfAtoms(FString Filename, bool OnlyAtom)
{
	return -1;
}

TArray<FString> UToolsFunctionLibrary::AcquireSymbols(FString Filename, bool OnlyAtom)
{
	return TArray<FString>();
}

TSet<FString> UToolsFunctionLibrary::UniqueElements(FString Filename)
{
	return TSet<FString>();
}

FVector UToolsFunctionLibrary::GetElementColourRGB(FString AtomSymbol)
{
	return FVector();
}

double UToolsFunctionLibrary::GetVDWRadius(FString AtomSymbol)
{
	return 0.0;
}
