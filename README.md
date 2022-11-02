# Element Editor
Element is a minimalistic voxel editor. This project began as an outlet to teach myself OpenGL and graphics programming in general, and over time I've continued 
to add features, with more anticipated in the future.

Currently, Element allows users to:
- Intuitively click to place, remove, and edit voxels
- Place and configure lights, with shading consiting of shadow mapping and ambient occlusion
- Export and import scenes as .element files

The design philosphy so far as been to always favor simplicity - there's not even support for text rendering as a symbol should work just as well. As the saying goes:
> Perfection is achieved, not when there is nothing more to add, but when there is nothing left to take away

![pagoda2](https://user-images.githubusercontent.com/30415201/199419451-edc70adb-6b1f-42f3-970b-58ebe381b598.png)

## Usage
Hold the middle mouse button to rotate the scene, use the scroll wheel to zoom, and shift + middle mouse button to pan.

Select the Add Block tool, and click anywhere adjacent to an existing block or the project boundaries to place a block. The color of the next placed block is set by
the RGB color sliders. To remove a block, select the Remove Block tool and click on a nearby block.

Select the Select Block tool, and click on blocks to add them to the current selection, which will be highlighted in orange. Shift + click to remove them from the 
selection. After making a selection, click the move tool and drag the translation handles to move the selection around within the project bounds. Selections
can also be extruded using the Extrude tool, with the position of the end of the extrusion movable with the translation handles.

Select the Add Light tool, and place lights just like blocks, setting the color first with the RGB sliders (intensity slider not yet implemented). Move lights with the
Move Light tool, and remove them with the Remove Light tool.

## Under the hood
This project is separated into two parts: the engine, which knows how to take in geometry and light data and render an image; and the editor, which encompasses the
UI, the application state and event system, and the scheme of data organization into blocks and chunks. These may one day be moved into separate projects, but for
now the editor is the engine's only client.

This project employs a common scheme in the voxel world, chunks. Each block belongs to a cube of blocks called a chunk, this allows changes to the mesh to be localized
to specific regions as needed.

The lighting in this project consists of a single directional light of configurable color, position, and angle, which illuminates the entire project and casts shadows;
up to 16 lights that are placeable and moveable in the scene but do not cast shadows; and a configurable ambient lighting. Ambient occlusion is implemented in a way that
provides a fast approximation leveraging the geometric constraints of the voxel world.

The UI architecture is very loosely based on Java AWT, and features several UI components that can be extended and combined into new client components.
