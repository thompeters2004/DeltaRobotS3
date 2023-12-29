#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct GCodeLine {
    std::string line;
    std::string commandType;
};

std::string extractValue(const std::string& line, char coordinate);

int main() {
    std::string gcode = "G1 X100 Y100 Z-220\n"
                        "G1 X10 Y10\n"
                        "G4 P200\n"
                        "G28\n"
                        "M04\n"
                        "M05\n"
                        "M204\n";

    std::vector<GCodeLine> relevantLines;
    size_t start = 0;

    // Recursive lambda to split G-code string
    std::function<void(size_t)> splitGCode = [&](size_t start) {
        size_t end = gcode.find("\n", start);
        if (end != std::string::npos) {
            std::string line = gcode.substr(start, end - start);
            std::string commandType = "";

            if (line.find("G1") != std::string::npos) {
                commandType = "G1";
            } else if (line.find("G4") != std::string::npos) {
                commandType = "G4";
            } else if (line.find("G28") != std::string::npos) {
                commandType = "G28";
            } else if (line.find("M04") != std::string::npos) {
                commandType = "M04";
            } else if (line.find("M05") != std::string::npos) {
                commandType = "M05";
            } else if (line.find("M204") != std::string::npos) {
                commandType = "M204";
            }

            relevantLines.push_back({line, commandType});
            splitGCode(end + 1);
        } else if (!gcode.substr(start).empty()) {
            relevantLines.push_back({gcode.substr(start), ""}); // Last part
        }
    };

    splitGCode(start);

    // Recursive lambda to process G-code lines
    std::function<void(size_t)> processGCodeLines = [&](size_t index) {
        if (index < relevantLines.size()) {
            const auto& gcodeLine = relevantLines[index];

            // Processing logic here...
            if (gcodeLine.commandType == "G1") {
                std::string xValue = extractValue(gcodeLine.line, 'X');
                std::string yValue = extractValue(gcodeLine.line, 'Y');
                std::string zValue = extractValue(gcodeLine.line, 'Z');

                std::cout << "G1 - Moving to X: " << xValue
                          << ", Y: " << yValue
                          << ", Z: " << zValue << std::endl;
            } else if (gcodeLine.commandType == "G4") {
                std::cout << "G4 - Delay" << std::endl;
            } else if (gcodeLine.commandType == "G28") {
                std::cout << "G28 - Home" << std::endl;
            } else if (gcodeLine.commandType == "M04") {
                std::cout << "M04 - Gripper ON" << std::endl;
            } else if (gcodeLine.commandType == "M05") {
                std::cout << "M05 - Gripper OFF" << std::endl;
            } else if (gcodeLine.commandType == "M204") {
                std::cout << "M204 - Set Speed" << std::endl;
            }

            processGCodeLines(index + 1);
        }
    };

    processGCodeLines(0);

    return 0;
}

std::string extractValue(const std::string& line, char coordinate) {
    std::size_t coordIndex = line.find(coordinate);
    if (coordIndex != std::string::npos) {
        coordIndex++; // Move past the coordinate character (e.g., 'X')
        std::size_t spaceIndex = line.find(' ', coordIndex);
        if (spaceIndex != std::string::npos) {
            return line.substr(coordIndex, spaceIndex - coordIndex);
        } else {
            return line.substr(coordIndex); // In case it's the last coordinate
        }
    }
    return ""; // Coordinate not found
}
