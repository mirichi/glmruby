class App
  def initialize(scene)
    @scene = scene
  end

  def update
    next_scene = @scene.update
    if Scene === next_scene
      scene.quit
      @scene = next_scene
    elsif next_scene == :exit
      scene.quit
      :exit
    end
  end

  def draw
    @scene.draw
  end
end
