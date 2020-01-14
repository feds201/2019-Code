 #pragma once
 
 #include <vector>
 #include <optional>
 
 #include <frc/util/Color.h>
 
 namespace rev {
 
 class ColorMatch {
 public:
     ColorMatch();
     void AddColorMatch(const frc::Color& color);
 
     void SetConfidenceThreshold(double confidence);
 
     std::optional<frc::Color> MatchColor(const frc::Color& colorToMatch);
 
     std::optional<frc::Color> MatchColor(const frc::Color& colorToMatch, double& confidence);
 
     frc::Color MatchClosestColor(const frc::Color& colorToMatch, double& confidence);
 
 private:
     std::vector<frc::Color> m_colorsToMatch;
     double m_confidenceLevel;
 };
 
 }
