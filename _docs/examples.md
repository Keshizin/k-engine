# Rendering with texture

In ```beforeMainLoopEvent```:

```
  auto q = kengine::quad(size, red, green, blue, alpha);
  kengine::render_node obj;
  obj.load(q);

  kengine::k_img img;
  img.loadfile("filename");

  kengine::texture tex;
  tex.create(kengine::TEXTURE_TYPE::_2D, 0, GL_RGBA8, img.getWidth(), img.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
  tex.bindTextureUnit(0); // bind to texture unit (hardware)
```

In ```update```:

```
  m_renderingSystem->clearBuffers();
  obj.draw();
```

# Instancing rendering

In ```beforeMainLoopEvent```:

```
  auto q = kengine::cube(size, red, green, blue, alpha);
  kengine::render_node obj;
  obj.load(q, TOTAL_ENTITY);
```

In ```update```:

```
  m_renderingSystem->clearBuffers();
  obj.drawInstanced(TOTAL_ENTITY);
```

# Rendering with Phong Lighting

In ```beforeMainLoopEvent```:

```
  auto q = kengine::quad(size, red, green, blue, alpha);
  kengine::render_node obj;
  obj.load(q, 1);

  kengine::uniform_buffer_object light;

  kengine::vec4<float> position = {0.0f, 50.0f, 0.0f};
  kengine::vec4<float> La = { 0.8f, 0.8f, 0.8f };
  kengine::vec4<float> Ld = { 0.8f, 0.8f, 0.8f };
  kengine::vec4<float> Ls = { 1.0f, 1.0f, 1.0f };

  std::vector<kengine::UBOVariable> lightParameters = {
    {"PhongLightInfo.position", position, 3},
    {"PhongLightInfo.La", La, 3},
    {"PhongLightInfo.Ld", Ld, 3},
    {"PhongLightInfo.Ls", Ls, 3}
  };

  light.load(m_shader.getProgramID(), "PhongLightInfo", lightParameters);

  kengine::uniform_buffer_object material;

  kengine::vec4<float> Ka = { 0.6f, 0.6f, 0.6f };
  kengine::vec4<float> Kd = { 0.8f, 0.8f, 0.8f };
  kengine::vec4<float> Ks = { 1.0f, 1.0f, 1.0f };
  kengine::vec4<float> shininess = { 120.0f, 1.0f, 1.0f };

  std::vector<kengine::UBOVariable> materialParameters = {
    {"PhongMaterialInfo.Ka", Ka, 3},
    {"PhongMaterialInfo.Kd", Kd, 3},
    {"PhongMaterialInfo.Ks", Ks, 3},
    {"PhongMaterialInfo.shininess", shininess, 1}
  };

  material.load(m_shader.getProgramID(), "PhongMaterialInfo", materialParameters);
```

In ```update```:

```
  m_renderingSystem->clearBuffers();
  obj.draw();
```
