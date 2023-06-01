import React from "react";
import ReactDOM from "react-dom"

export function Home(){
    return(
        <div className="w-full ">
            <div className="w-5/12 h-80 ml-10">
                <h1 className="text-white text-6xl font-extrabold">Unleash the power of our tires with the help of this platform.</h1>
                <div className="ml-1 flex flex-row items-center mt-8">
                    <div className="w-40 h-1 bg-white"></div>
                    <p className="text-2xl ml-3 font-extrabold text-gold">Who we are?</p>
                </div>
                <div className="text-gray-1 font-bold text-xl mt-11"> We are intelli college students. We created this
                    tablet tracking and monitoring
                    platform for Pirelli. </div>
            </div>
        </div>
    )
}