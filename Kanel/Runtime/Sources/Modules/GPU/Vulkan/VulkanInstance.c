// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Vulkan/VulkanInstance.h>
#include <Modules/GPU/Vulkan/VulkanPrototypes.h>

#include <Config.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static VkDebugUtilsMessengerEXT __kbh_debug_messenger = VK_NULL_HANDLE;
static char** __kbh_extra_layers = KANEL_CLI_NULLPTR;
static size_t __kbh_extra_layers_count = 0;

#ifdef KANEL_CLI_VULKAN_DEBUG
	static bool kbhVulkanCheckValidationLayerSupport()
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, KANEL_CLI_NULLPTR);
		VkLayerProperties* available_layers = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
		for(size_t i = 0; i < __kbh_extra_layers_count; i++)
		{
			bool found = false;
			for(size_t j = 0; j < layer_count; j++)
			{
				if(strcmp(available_layers[j].layerName, __kbh_extra_layers[i]) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				free(available_layers);
				return false;
			}
		}
		free(available_layers);
		return true;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL kbhVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			kbhErrorFmt("\nVulkan validation error : %s\n", pCallbackData->pMessage);
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			kbhWarningFmt("\nVulkan validation warning : %s\n", pCallbackData->pMessage);
		return VK_FALSE;
	}

	static void kbhVulkanPopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* create_info)
	{
		create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info->pfnUserCallback = kbhVulkanDebugCallback;
	}

	static VkResult kbhVulkanCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, VkDebugUtilsMessengerEXT* messenger)
	{
		PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		return func ? func(instance, create_info, KANEL_CLI_NULLPTR, messenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void kbhVulkanInitValidationLayers(VkInstance instance)
	{
		uint32_t extension_count;
		vkEnumerateInstanceExtensionProperties(KANEL_CLI_NULLPTR, &extension_count, KANEL_CLI_NULLPTR);
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extension_count * sizeof(VkExtensionProperties));
		vkEnumerateInstanceExtensionProperties(KANEL_CLI_NULLPTR, &extension_count, extensions);
		bool extension_found = false;
		for(uint32_t i = 0; i < extension_count; i++)
		{
			if(strcmp(extensions[i].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				extension_found = true;
				break;
			}
		}
		if(!extension_found)
		{
			kbhWarning("Vulkan warning: " VK_EXT_DEBUG_UTILS_EXTENSION_NAME " is not present; cannot enable validation layers");
			free(extensions);
			return;
		}
		VkDebugUtilsMessengerCreateInfoEXT create_info = {};
		kbhVulkanPopulateDebugMessengerCreateInfo(&create_info);
		kbhCheckVk(kbhVulkanCreateDebugUtilsMessengerEXT(instance, &create_info, &__kbh_debug_messenger));
	}

	static void kbhVulkanDestroyDebugUtilsMessengerEXT(VkInstance instance)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if(func)
			func(instance, __kbh_debug_messenger, KANEL_CLI_NULLPTR);
	}
#endif

void kbhVulkanAddLayer(const char* layer)
{
	#ifdef KANEL_CLI_VULKAN_DEBUG
		__kbh_extra_layers = (char**)realloc(__kbh_extra_layers, sizeof(char*) * (__kbh_extra_layers_count + 1));
		kbhAssert(__kbh_extra_layers != KANEL_CLI_NULLPTR);
		__kbh_extra_layers[__kbh_extra_layers_count] = (char*)malloc(strlen(layer) + 1);
		kbhAssert(__kbh_extra_layers[__kbh_extra_layers_count] != KANEL_CLI_NULLPTR);
		strcpy(__kbh_extra_layers[__kbh_extra_layers_count], layer);
		__kbh_extra_layers_count++;
	#else
		KANEL_CLI_UNUSED(layer);
	#endif
}

VkInstance kbhVulkanCreateInstance(const char** extensions_enabled, uint32_t extensions_count)
{
	VkInstance instance = VK_NULL_HANDLE;

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pEngineName = "kanel-CLI";
	app_info.engineVersion = VK_MAKE_VERSION(KANEL_CLI_VERSION_MAJOR, KANEL_CLI_VERSION_MINOR, KANEL_CLI_VERSION_PATCH);
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pApplicationName = "kanel-CLI";
	app_info.applicationVersion = VK_MAKE_VERSION(KANEL_CLI_VERSION_MAJOR, KANEL_CLI_VERSION_MINOR, KANEL_CLI_VERSION_PATCH);

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = extensions_count;
	create_info.ppEnabledExtensionNames = extensions_enabled;
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = KANEL_CLI_NULLPTR;
	create_info.pNext = KANEL_CLI_NULLPTR;
	#ifdef KANEL_CLI_PLAT_MACOS
		create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	#else
		create_info.flags = 0;
	#endif

#ifdef KANEL_CLI_VULKAN_DEBUG
	kbhVulkanAddLayer("VK_LAYER_KHRONOS_validation");
	const char** new_extension_set = KANEL_CLI_NULLPTR;
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
	if(kbhVulkanCheckValidationLayerSupport())
	{
		kbhVulkanPopulateDebugMessengerCreateInfo(&debug_create_info);
		new_extension_set = (const char**)malloc(sizeof(char*) * (extensions_count + 1));
		memcpy(new_extension_set, extensions_enabled, sizeof(char*) * extensions_count);
		new_extension_set[extensions_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

		create_info.enabledExtensionCount = extensions_count + 1;
		create_info.ppEnabledExtensionNames = new_extension_set;
		create_info.enabledLayerCount = __kbh_extra_layers_count;
		create_info.ppEnabledLayerNames = (const char* const*)__kbh_extra_layers;
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	}
#endif

	kbhCheckVk(vkCreateInstance(&create_info, KANEL_CLI_NULLPTR, &instance));
#ifdef KANEL_CLI_VULKAN_DEBUG
	free(new_extension_set);
	kbhVulkanInitValidationLayers(instance);
#endif
	kbhDebugLog("Vulkan : instance created");
	return instance;
}

void kbhVulkanDestroyInstance(VkInstance instance)
{
	if(instance == VK_NULL_HANDLE)
		return;
#ifdef KANEL_CLI_VULKAN_DEBUG
	kbhVulkanDestroyDebugUtilsMessengerEXT(instance);
	for(size_t i = 0; i < __kbh_extra_layers_count; i++)
		free(__kbh_extra_layers[i]);
	free(__kbh_extra_layers);
	__kbh_extra_layers_count = 0;
#endif
	vkDestroyInstance(instance, KANEL_CLI_NULLPTR);
	kbhDebugLog("Vulkan : instance destroyed");
}
