#include "Renderer.h"
#include <vulkan/vk_enum_string_helper.h>
#include "../IO/Window.h"
#include <thread>
#include "../Threads/ThreadManager.h"
#include "../Landmark.h"
#include "../IO/WindowManager.h"


namespace Landmark
{
	class Engine;
}

namespace Landmark
{
	namespace Render
	{
		Renderer::Renderer()
		{
			
			
		}
	
		std::string Renderer::GetName() const
		{
			return std::string("RenderLogic");
		}

		void Renderer::PreInit()
		{
			LOGGER.Log("Init");
			
			



			SubscribeTo<Vk::Event_GpuTaskRequest>([&](Vk::Event_GpuTaskRequest& e) {
				//LOGGER.Log("Requesting Main Rendering Task on Device 0 (HARDCODED) Type: " + std::string(string_VkPhysicalDeviceType(e.AvailableDevices[0].deviceProperties.deviceType)));
				

				Vk::Event_GpuTaskRequest::TaskRequest request = {
					"Renderer Main Task",
					Vk::Task::TaskIntensities::VERY_HIGH,
					e.AvailableDevices[0].DeviceID,
					Vk::GRAPHICS | Vk::COMPUTE | Vk::TRANSFER | Vk::SPARSE_BINDING,
					{VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME},
					{}

				};

				RenderingTask = e.DeclareTask(request);
				});
			
			
			
			///RenderingThread.join();
			
		}
		void Renderer::Init() 		
		{
			RenderingThread = std::thread(RenderingThreadStart);
			LOGGER.Log("Rendering Thread Dispatched");
		}
		void Renderer::PostInit() 
		{

		}

		void Renderer::RenderingThreadStart()
		{
			
			LOGGER.Log("Rendering Thread Init");
			//ainWindow->MakeCurrent();

			while (!CloseThread)
			{
				if (IO::WindowManager::MainWindow().GetShouldClose())
					Engine::Shutdown();
			}
		}

		void Renderer::Exit()
		{
			CloseThread = true;
			RenderingThread.join();
			LOGGER.Log("Rendering Thread Closed");
		}
	}
}
