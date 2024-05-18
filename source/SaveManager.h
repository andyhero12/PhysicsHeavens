//
//  SaveManager.hpp
//  Heaven
//
//  Created by Colin Wu on 5/17/24.
//

#ifndef SaveManager_h
#define SaveManager_h

#include <stdio.h>
#include <cugl/cugl.h>
using namespace cugl;

class SaveManager{

private:
    /** The path to the save file */
    std::string path;
    
    /** Reader to read from the save file */
    std::shared_ptr<cugl::JsonReader> _reader;
    
public:
    SaveManager(){
        std::string root_path = cugl::Application::get()->getSaveDirectory();
        path = cugl::filetool::join_path({root_path,"save.json"});
        _reader = JsonReader::alloc(path);
    };
    
    
    /** Initializes save file values if one doesn't exist */
    void init();
    
    
    void write(const std::shared_ptr<JsonValue>& json);
    
    /** Get the root of the save file JSON */
    std::shared_ptr<JsonValue> read();
    
    /**
     * Disposes the manager, releasing all resources.
     */
    ~SaveManager() {};//_reader->close();
};


#endif /* SaveManager_h */
