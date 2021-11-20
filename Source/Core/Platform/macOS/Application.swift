//
//  Application.swift
//  BGFXDemo
//
//  Created by Pierre on 14/11/2021.
//

import Cocoa
import Foundation

public class Application {
    
    private var _run = false
    
    public func shouldRun() -> Bool{
        
        return _run;
        
    }
    
    public func run(args: [String]) {
        
        _ = NSApplication.shared
        NSApp.setActivationPolicy(NSApplication.ActivationPolicy.regular)
        NSApp.activate(ignoringOtherApps: true)
        NSApp.finishLaunching()
        _run = true
        
        let window = WindowManager.instance.createWindow(x: 0, y: 0, w: 1280, h: 720, title: "Swift Window", monitor: 0);
        
        DispatchQueue.global(qos: .userInitiated).async {

            Bridge.initEngine(window)
            
        }
        
        while(shouldRun()){
            
            loop();
            
        }
                
    }
    
    private func loop(){
        
        Bridge.renderFrame()
        
        let event = getNextEvent()
        
        if(event != nil){
            
            NSApp.sendEvent(event!)
            
        }
        
        NSApp.updateWindows()
        
    }
    
    public func getNextEvent() -> NSEvent? {
        
        return NSApp.nextEvent(matching: .any, until: .distantPast, inMode: .defaultRunLoopMode, dequeue: true)
        
    }
    
    
}
