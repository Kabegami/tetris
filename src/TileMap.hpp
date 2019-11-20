#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

using namespace std;
using namespace Eigen;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
  bool load(const std::string& tileset, sf::Vector2u tileSize, sf::Vector2u displaySize, MatrixXf tiles);
private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::VertexArray m_vertices;
  sf::Texture m_tileset;
};
