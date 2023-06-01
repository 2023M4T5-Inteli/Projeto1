import React from 'react'
import ReactDOM from 'react-dom'
import logoPirelli from '../assets/Logo_Pirelli.svg'
import '../global.css'

export function Navbar(){
    return(
        <nav className="w-full h-24 bg-transparent flex flex-row items-center justify-between p-5">
        <div className="w-30 h-10 bg-white">
            <img src={logoPirelli} className='w-full h-full'></img>
        </div>

        <div className='flex flex-row items-center w-4/12 justify-between'>
            <a href="#" className="text-white text-xl">Add Device</a>
            <a href="#" className="text-white text-xl">Maps</a>
            <a href="#" className="text-white text-xl">Dashboard</a>
        </div>
        <div>
            <a href="#" className="text-white text-xl">Login</a>
        </div>
    </nav>
    )
}