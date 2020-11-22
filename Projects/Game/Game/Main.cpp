#include "Game/Application.h"

#include <Engine/FileHelpers.h>
#include <Engine/JsonHelpers.h>
#include <Engine/Path.h>
#include <Engine/String.h>
#include <Engine/StringHelpers.h>

#include <direct.h>

namespace
{
	bool GenerateAsset(const str::Path& inputPath, const str::Path& outputPath)
	{
		str::String resourceGuid = str::GenerateGUID();

		str::String binaryData;
		core::LoadFileAsBinary(inputPath, binaryData);
		rapidjson::SizeType binarySize = static_cast<rapidjson::SizeType>(binaryData.size());

		rapidjson::Document document;
		rapidjson::Value resource_guid;
		rapidjson::Value resource_type;
		rapidjson::Value source_file;
		rapidjson::Value binary_data;

		document.SetObject();
		resource_guid.SetString(resourceGuid.c_str(), document.GetAllocator());
		resource_type.SetString("texture", document.GetAllocator());
		source_file.SetString(inputPath.ToChar(), document.GetAllocator());
		binary_data.SetString(binaryData.c_str(), binarySize, document.GetAllocator());

		document.AddMember("resource_guid", resource_guid, document.GetAllocator());
		document.AddMember("resource_type", resource_type, document.GetAllocator());
		document.AddMember("source_file", source_file, document.GetAllocator());
		document.AddMember("binary_data", binary_data, document.GetAllocator());

		json::SaveDocument(outputPath.ToChar(), document);

		return true;
	}
}

int main(int agrc, char* argv[])
{
	_chdir("../../");

	game::Application* application = new game::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}