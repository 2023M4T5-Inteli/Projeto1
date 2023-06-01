import {
    BrowserRouter as Router, 
    Route,
    Routes
    }  from "react-router-dom";
import { PaginaInicial } from "./pages/pagina_inicial";

    
export function AppRoutes(){
    return (
        <Router>
            <Routes>
                <Route path="/" element={<PaginaInicial />}>
                    </Route>
            </Routes>   
        </Router>
    )
}