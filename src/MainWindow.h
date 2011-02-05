#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreWindowEventUtilities.h>

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <QObject>
#include <QHash>
#include <QSharedPointer>

#include "Chunk.h"
#include "Server.h"

class MainWindow :
    public QObject,
    public Ogre::FrameListener,
    public Ogre::WindowEventListener,
    public OIS::KeyListener,
    public OIS::MouseListener
{
    Q_OBJECT
public:
    enum Control {
        Forward,
        Back,
        Left,
        Right,
        Jump,
        Crouch,
        DiscardItem,
        Action1, // left click
        Action2, // right click
        Inventory,
        Chat,
    };

public:
    MainWindow();
    ~MainWindow();

    int exec();

protected:
    // Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    void windowClosed(Ogre::RenderWindow* rw);

private:
    static const Int3D c_side_offset[];
    static const Int3D c_chunk_size;
    static const Ogre::Vector3 c_side_coord[6][2][3];
    static const Ogre::Vector2 c_tex_coord[2][3];

    Ogre::Root *m_root;
    Ogre::Camera* m_camera;
    Ogre::SceneManager* m_scene_manager;
    Ogre::RenderWindow* m_window;
    Ogre::String m_resources_config;

    bool m_shut_down;

    //OIS Input devices
    OIS::InputManager* m_input_manager;
    OIS::Mouse* m_mouse;
    OIS::Keyboard* m_keyboard;

    struct Mob {
        Ogre::Vector3 pos;
        Ogre::Vector3 up;
        Ogre::Vector3 look;
        double stance;
        bool on_ground;
    };

    struct BlockTextureCoord {
        int x;
        int y;
        int w;
        int h;
    };

    Server * m_server;

    // key is the meter coordinates of the min corner
    QHash<Int3D, QSharedPointer<Chunk> > m_chunks;
    QHash<qint32, QSharedPointer<Mob> > m_entities;

    Mob * m_player;

    // maps OIS::KeyCode to Control and vice versa
    QHash<OIS::KeyCode, Control> m_key_to_control;
    QHash<Control, OIS::KeyCode> m_control_to_key;

    // maps texture name to coordinates in terrain.png
    QHash<QString, BlockTextureCoord> m_terrain_tex_coords;
    // maps item type to texture name for each side
    QHash<Chunk::ItemType, QVector<QString> > m_side_texture_names;

private:
    void loadControls();

    bool setup();
    bool configure();
    void createCamera();
    void createFrameListener();
    void createViewports();
    void setupResources();
    void loadResources();

    Chunk::ItemType blockTypeAt(const Int3D & coord);
    Int3D chunkKey(const Int3D & coord);
    void generateChunkMesh(QSharedPointer<Chunk>);
    QSharedPointer<Chunk> getChunk(const Int3D & );

private slots:
    void updateChunk(QSharedPointer<Chunk> chunk);
    void movePlayerPosition(Server::EntityPosition position);

};

#endif // MAINWINDOW_H