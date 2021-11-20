//
//  Platform_macOS.swift
//  BGFXDemo
//
//  Created by Pierre on 11/11/2021.
//

import Foundation
import AppKit

fileprivate class Window : NSWindow{
    
    fileprivate override func keyDown(with event: NSEvent) {
               
        print(event.characters!)
        
    }
        
}

public struct WindowHandle{
    
    var __id : Int
    
}

public class WindowManager {
    
    private class EventHandler : NSObject, NSWindowDelegate{
        
        public func windowWillClose(_ notification: Notification) {
            
            if(notification.object == nil) { return }
            
            let targetWindow = notification.object! as? NSWindow
            
            if(targetWindow == nil) { return }
            
            targetWindow!.delegate = nil
                                    
            DispatchQueue.main.async {
                
                let index = WindowManager.instance.windows.firstIndex(of: targetWindow!)
                WindowManager.instance.windows.remove(at: index!)
                                
                if(index! == 0){
                    NSApplication.shared.terminate(self)
                }
                                
            }
                    
        }
                
    }
    
    public static let instance = WindowManager()
    private let semaphore = DispatchSemaphore(value: 1)
    private let delegate: EventHandler = EventHandler()
    private var windows: [NSWindow] = []
    
    public func destroyAllWindows(){
        
        for _ in 0 ..< windows.count {
            windows.remove(at: 0)
        }
        
    }
        
    public func destroyWindow(handle: WindowHandle){
                
        if(!Thread.current.isMainThread){
            
            DispatchQueue.main.async {
                
                self.destroyWindow(handle: handle)
                
            }
            
            return
            
        }
        
        var window : NSWindow? = nil
        var windowIndex : Int = -1
        
        for i in 0 ..< windows.count {
            
            if(windows[i].windowNumber == handle.__id){
                
                window = windows[i]
                windowIndex = i
                break
                
            }
            
        }
        
        if(window == nil) { return }
        
        window!.close()
        windows.remove(at: windowIndex)
        
    }
        
    public func createWindow(x: Int, y: Int, w: Int, h: Int, title: String, monitor: Int) -> NSWindow{
                
        semaphore.wait()
        
        var window : NSWindow? = nil
        
        if(Thread.isMainThread){
            
            window = _createWindow(x: x, y: y, w: w, h: h, title: title, monitor: monitor)
            
        }
        else{
            
            DispatchQueue.main.sync {
                window = _createWindow(x: x, y: y, w: w, h: h, title: title, monitor: monitor)
            }
                        
        }
        
        windows.append(window!)
        
        semaphore.signal()

        return window!
        
    }
    
    private func _createWindow(x: Int, y: Int, w: Int, h: Int, title: String, monitor: Int) -> NSWindow{
        
        let rect = NSRect(x: x, y: y, width: w, height: h)
        let mask : NSWindow.StyleMask = [.closable, .resizable, .titled]
        let screen : NSScreen? = NSScreen.screens.count < monitor ? nil : NSScreen.screens[monitor]
        let window = Window(contentRect: rect, styleMask: mask, backing: NSWindow.BackingStoreType.buffered, defer: false, screen: screen)
        window.delegate = delegate
        
        /**
         * This flag MUST be set to false because of ARC. ARC will release the window when no more reference to the object exists.
         * If we set the window to be automatically released on close, ARC will try to re-release the object when no more refs
         * to it exists, causing a runtime error (double free)
         **/
        window.isReleasedWhenClosed = false
        window.title = title
        window.makeKeyAndOrderFront(NSApplication.shared)
        
        return window
        
    }
    
}
