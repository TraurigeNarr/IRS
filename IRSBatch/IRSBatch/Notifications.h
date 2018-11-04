
#pragma once

//inherit this class
//and then by dynamic casts ;)
class INotificationData
{
public:
  virtual ~INotificationData()
  {
  }
};

class Vertex;
class ClusterRenderer;
class ClassHandlerRenderer;
class IRenderable;

class VertexAdded : public INotificationData
{
  Vertex* mp_vertex;

  VertexAdded(Vertex* ip_vertex)
    : mp_vertex(ip_vertex)
  {

  }
};

class ClassAdded : public INotificationData
{
public:
  ClassHandlerRenderer* mp_class;

  ClassAdded(ClassHandlerRenderer* ip_class)
    : mp_class(ip_class)
  {

  }
};

class ClusterAdded : public INotificationData
{
public:
  ClusterRenderer* mp_cluster;

  ClusterAdded(ClusterRenderer* ip_cluster)
    : mp_cluster(ip_cluster)
  {

  }
};

class ItemChanged : public INotificationData
{
public:
  enum ItemState
  {
    IS_ADDED,
    IS_REMOVED,
    IS_CHANGE_STATE
  };

  IRenderable* mp_renderable;
  ItemState m_state;

  ItemChanged(IRenderable* ip_renderable, ItemState i_state)
    : mp_renderable(ip_renderable)
    , m_state(i_state)
  {

  }
};

class DatabaseCleared : public INotificationData
{

};

class ClusteringSucceeded : public INotificationData
{
};

class ItemSelectionChange : public INotificationData
{
public:
  enum ItemState
  {
    IS_SELECTED,
    IS_DESELECTED
  };

  IRenderable* mp_renderable;
  ItemState m_state;

  ItemSelectionChange(IRenderable* ip_renderable, ItemState i_state)
    : mp_renderable(ip_renderable)
    , m_state(i_state)
  {

  }
};

class RecognitionSucceded : public INotificationData
{
};

//////////////////////////////////////////////////////////////////////////

class INotificationHandler
{
  bool m_listening;
public:
  INotificationHandler()
  {
    StartListen();
  }

  virtual ~INotificationHandler(){}

  void StartListen()
  {
    m_listening = true;
  }

  void StopListen()
  {
    m_listening = false;
  }

  bool IsListening() const
  {
    return m_listening;
  }

  virtual void HandleNotification(INotificationData* ip_data) = 0;
};