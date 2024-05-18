//
//  SaveManager.cpp
//  Heaven
//
//  Created by Colin Wu on 5/17/24.
//

#include "SaveManager.h"

using namespace cugl;

void SaveManager::init(){
    
    // Create the save file if it doesn't exist
    if(!cugl::filetool::file_exists(path)){
        CULog("NO SAVE FILE FOUND, CREATING SAVE FILE");
        
        cugl::filetool::file_create(path);
        
        // Write initial save root
        std::shared_ptr<JsonValue> root = std::make_shared<JsonValue>();
        root->init(JsonValue::Type::ObjectType);
    
        write(root);
    }
    
    // Initialize json reader/writer
    std::shared_ptr<JsonValue> json_root = read();
    
    // Initialize values that don't exist
    if(!json_root->has("unlocked")){
        json_root->appendValue("unlocked", (long) 1);
    }
    
    write(json_root);
}

void SaveManager::write(const std::shared_ptr<JsonValue>& json){
    std::shared_ptr<JsonWriter> _writer = JsonWriter::alloc(path);
    _writer->writeJson(json);
    _writer->close();
}

std::shared_ptr<JsonValue> SaveManager::read(){
    std::shared_ptr<JsonReader> reader = JsonReader::alloc(path);
    std::shared_ptr<JsonValue> json = reader->readJson();
    reader->close();
    return json;
}
