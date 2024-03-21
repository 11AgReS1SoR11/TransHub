#include "FreeWindowManager.h"

//FreeWindowManager::FreeWindowManager (QMdiArea *mdiArea)
//    : _mdiArea (mdiArea)
//{

//}

//FreeWindowManager::~FreeWindowManager()
//{
//    clear ();
//}

//FreeWindow* FreeWindowManager::addWindow (QWidget* widget)
//{
//    if (!widget)
//        return 0x00;

//    QPointer<FreeWindow> pointer = new FreeWindow ();
//    _windows.insert (widget, pointer);
//    pointer->goInside ();

//    return pointer;

////    {
////        auto free_window = std::unique_ptr<FreeWindow>( new FreeWindow( widget, m_internal_area ) );
////        m_windows_registry.insert( std::make_pair( &widget, std::move( free_window ) ) );
////    }

////    {
////        auto& free_window = *m_windows_registry[ &widget ];
////        free_window.go_inside();

////        return free_window;
////    }
//}

//void FreeWindowManager::removeWindow (QWidget *widget)
//{
//    if (widget)
//        _windows.remove (widget);

//    // TODO : do it in one search instead of two
////    auto free_window = findWindow( widget );
////    if( free_window )
////    {
////        m_windows_registry.erase( &widget );
////    }
//}

//void FreeWindowManager::clear()
//{
//    _windows.clear ();
////    m_windows_registry.clear();
//}

//FreeWindow* FreeWindowManager::findWindow (QWidget *widget)
//{
//    QPointer<FreeWindow> pointer = _windows.value (widget);
//    if (!pointer.isNull ())
//        return pointer;
//    else
//        return 0x00;

////    auto free_window_it = m_windows_registry.find( &widget );
////    if( free_window_it != m_windows_registry.end() )
////        return free_window_it->second.get();
////    else
////        return nullptr;
//}
