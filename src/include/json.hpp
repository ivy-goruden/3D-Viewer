Json::Value createJson() {
    Json::Value root;
    
    // Простые типы
    root["name"] = "MyModel";
    root["version"] = 1;
    root["coefficient"] = 3.14;
    root["active"] = true;
    
    // Массив
    Json::Value arr(Json::arrayValue);
    arr.append(1);
    arr.append(2);
    arr.append(3);
    root["data"] = arr;
    
    // Вложенный объект
    root["settings"]["indent"] = 4;
    root["settings"]["use_spaces"] = true;
    
    return root;
}

std::string serializeToString(const Json::Value& root) {
    // Вариант 1: StyledWriter — форматированный вывод (для чтения) [citation:8]
    Json::StyledWriter styledWriter;
    std::string styled = styledWriter.write(root);
    
    // Вариант 2: FastWriter — без форматирования, в одну строку [citation:8]
    Json::FastWriter fastWriter;
    std::string compact = fastWriter.write(root);
    
    // Вариант 3: StreamWriterBuilder — с контролем форматирования (рекомендуется) [citation:3]
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";  // 2 пробела для отступа
    return Json::writeString(builder, root);
}

bool deserializeFromString(const std::string& jsonStr, Json::Value& root) {
    // Для JsonCpp 1.x (устаревший, но всё ещё распространён)
    Json::Reader reader;
    if (reader.parse(jsonStr, root)) {
        return true;
    } else {
        std::cerr << "Parse error: " << reader.getFormattedErrorMessages() << std::endl;
        return false;
    }
}

bool saveToFile(const Json::Value& root, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &file);
    return true;
}

bool loadFromFile(const std::string& filename, Json::Value& root) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    Json::CharReaderBuilder builder;
    std::string errs;
    return Json::parseFromStream(builder, file, &root, &errs);
}
