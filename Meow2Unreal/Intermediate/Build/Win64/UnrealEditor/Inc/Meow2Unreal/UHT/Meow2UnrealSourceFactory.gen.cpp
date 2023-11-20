// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Meow2Unreal/Private/Meow2UnrealSourceFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMeow2UnrealSourceFactory() {}
// Cross Module References
	LIVELINKINTERFACE_API UClass* Z_Construct_UClass_ULiveLinkSourceFactory();
	MEOW2UNREAL_API UClass* Z_Construct_UClass_UMeow2UnrealSourceFactory();
	MEOW2UNREAL_API UClass* Z_Construct_UClass_UMeow2UnrealSourceFactory_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Meow2Unreal();
// End Cross Module References
	void UMeow2UnrealSourceFactory::StaticRegisterNativesUMeow2UnrealSourceFactory()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMeow2UnrealSourceFactory);
	UClass* Z_Construct_UClass_UMeow2UnrealSourceFactory_NoRegister()
	{
		return UMeow2UnrealSourceFactory::StaticClass();
	}
	struct Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ULiveLinkSourceFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_Meow2Unreal,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Meow2UnrealSourceFactory.h" },
		{ "ModuleRelativePath", "Private/Meow2UnrealSourceFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMeow2UnrealSourceFactory>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::ClassParams = {
		&UMeow2UnrealSourceFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UMeow2UnrealSourceFactory()
	{
		if (!Z_Registration_Info_UClass_UMeow2UnrealSourceFactory.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMeow2UnrealSourceFactory.OuterSingleton, Z_Construct_UClass_UMeow2UnrealSourceFactory_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMeow2UnrealSourceFactory.OuterSingleton;
	}
	template<> MEOW2UNREAL_API UClass* StaticClass<UMeow2UnrealSourceFactory>()
	{
		return UMeow2UnrealSourceFactory::StaticClass();
	}
	UMeow2UnrealSourceFactory::UMeow2UnrealSourceFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMeow2UnrealSourceFactory);
	UMeow2UnrealSourceFactory::~UMeow2UnrealSourceFactory() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_OpenTafl_Plugins_Meow2Unreal_Source_Meow2Unreal_Private_Meow2UnrealSourceFactory_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_OpenTafl_Plugins_Meow2Unreal_Source_Meow2Unreal_Private_Meow2UnrealSourceFactory_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMeow2UnrealSourceFactory, UMeow2UnrealSourceFactory::StaticClass, TEXT("UMeow2UnrealSourceFactory"), &Z_Registration_Info_UClass_UMeow2UnrealSourceFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMeow2UnrealSourceFactory), 3945127551U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_OpenTafl_Plugins_Meow2Unreal_Source_Meow2Unreal_Private_Meow2UnrealSourceFactory_h_2621546088(TEXT("/Script/Meow2Unreal"),
		Z_CompiledInDeferFile_FID_UnrealProjects_OpenTafl_Plugins_Meow2Unreal_Source_Meow2Unreal_Private_Meow2UnrealSourceFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_OpenTafl_Plugins_Meow2Unreal_Source_Meow2Unreal_Private_Meow2UnrealSourceFactory_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
