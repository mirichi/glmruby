class TestSprite < Sprite
  def update
    @x+=Input.x
    @y+=Input.y
  end
end

class TestScene < Scene
  def initialize
    @sprites = [TestSprite.new(100,100,"./image/ruby.png"),
                TestSprite.new(100,200,"./image/ruby.png")]
  end

  def update
    @sprites.each do |s|
      s.update
    end
  end

  def draw
    @sprites.each do |s|
      s.draw
    end
  end
end


App.new(TestScene.new)
