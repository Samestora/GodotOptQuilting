extends Node2D

@export var source_path: String = "res://source_texture.png"
@export var output_width: int = 256
@export var output_height: int = 256
@export var patch_size: int = 32
@export var overlap: int = 8
@export_range(0.0, 1.0, 0.01) var tolerance: float = 0.1
@export var seed: int = 12345

var _quilting: ImageQuilting

func _ready() -> void:
    _quilting = ImageQuilting.new()

    var source := Image.load_from_file(source_path)
    if source == null or source.is_empty():
        push_error("Cannot load source texture: " + source_path)
        return

    var start_usec := Time.get_ticks_usec()
    var result: Image = _quilting.synthesize(
        source,
        output_width,
        output_height,
        patch_size,
        overlap,
        tolerance,
        seed
    )
    var elapsed_usec := Time.get_ticks_usec() - start_usec

    if result == null or result.is_empty():
        push_error("Image Quilting failed.")
        return

    var err := result.save_png("res://quilting_output.png")
    print("Image Quilting elapsed: ", elapsed_usec / 1000.0, " ms")
    print("Saved result: ", ProjectSettings.globalize_path("res://quilting_output.png"),
          " (Error code: ", err, ")")

    var texture := ImageTexture.create_from_image(result)
    var sprite := Sprite2D.new()
    sprite.texture = texture
    sprite.position = Vector2(output_width, output_height) * 0.5
    add_child(sprite)
