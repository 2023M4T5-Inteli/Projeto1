import React from 'react'
import ReactDOM from 'react-dom'
import '../global.css'
import { Navbar } from '../components/Navbar'
import { Home } from '../components/Home'

function MeetUsClick(){
    const scrollElement= document.getElementById("who-we-are").scrollIntoView();
    if (scrollElement){
        const offset= scrollElement.offsetTop;

        window.scrollTo({
            top: offset,
            behavior: 'smooth'
        });
    }
};

export function PaginaInicial(){
    return(
        <main className=' bg-gradient-black w-full h-bsize'>
            <div>
                <Navbar />
            </div>
            <div className='w-full h-default-1 mt-10'>
                <Home />
                <div className='flex flex-col items-center justify-center w-full h-20 gradient-1 mt-default-1 hover:cursor-pointer' onClick={MeetUsClick}>
                    <p className='text-2xl font-bold text-white'>Nos conheça melhor</p>
                    <i className="arrow-down"></i>
                </div>
            </div>
            <div id="who-we-are" className='mt-30 ml-10 mr-10'>
                <h1 className='text-gold font-bold text-4xl'>Who we are?</h1>
                <p className='text-white font-bold mt-5'>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do 
                eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim 
                ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip
                ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate
                velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat 
                cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est 
                laborum.
                </p>
            </div>
            <div className='mt-20 ml-10 mr-10 flex flex-col items-center'>
                <h1 className='text-gold font-bold text-4xl'>Members</h1>
                <div className='mt-10 grid grid-template-cols-default gap-10 justify-center'>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Arthur Oliveira</p>
                    </div>
                     <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Felipe Moura</p>
                    </div>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Guilherme Moura</p>
                    </div>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Samuel Lucas</p>
                    </div>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Tony Jonas</p>
                    </div>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Vinicius Kumagai</p>
                    </div>
                    <div className='card-main'>
                        <div className='card-photo'></div>
                        <p className='mt-5 font-bold text-3xl text-white ml-1'>Yago Matos</p>
                    </div>
                    
                </div>
            </div>
         
        </main>
        
    )
}