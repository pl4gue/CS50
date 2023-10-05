const GameState = {
    paused: 'paused',
    playing: 'playing'
}

var Game = {
    state: GameState.paused,
    grid: [],
    fps: 30
}

let cellSize = 20;
let gridHeight = 35;
let gridWidth = 100;
    

function init() {
    // Populate the game grid with false values
    Game.grid = []

    for (let h = 0; h < gridWidth; h++) {
      let newRow = [];

      for (let w = 0; w < gridHeight; w++) {
        newRow.push(false);
      }

      Game.grid.push(newRow);
    }
}

init()
    
function mousePressed() {
    // Get row and column of clicked cell
    let col = (mouseX - (mouseX % cellSize)) / cellSize
    let row = (mouseY - (mouseY % cellSize)) / cellSize

    // Check for valid cell
    if (col <= gridWidth && row < gridHeight) 
    {
        // Invert the cell value and render the game again
        try {
            Game.grid[col][row] = !Game.grid[col][row]
        } catch (error) {
            
        }

        draw()
    }
}

function startGame() {
    const isPaused = Game.state == GameState.paused;

    Game.fps = isPaused ? 2 : 30;
    frameRate(Game.fps);
    Game.state = isPaused ? GameState.playing : GameState.paused;

    let button = document.querySelector(".game-button")
    button.innerHTML = Game.state == GameState.paused ? "Continue Game" : "Pause Game"
}

// Runs on start
function setup() { 
    frameRate(Game.fps);
    createCanvas(innerWidth, 800);
}

function countLiveNeighbors(row, col) {
    let count = 0;
    
    for (let h = -1; h < 2; h++) {

        if (!(col + h >= 0 && col + h < gridWidth - 1)) continue;

        if (row > 0 && Game.grid[row - 1][col + h] == 1) {
            count++;
        }

        if (row < gridWidth - 1 && Game.grid[row + 1][col + h] == 1) { 
            count++;
        }
    }
    
    if (col - 1 >= 0 && Game.grid[row][col - 1] == 1) { // Check left cell
        count++;
    }
    if (col + 1 < gridHeight - 1 && Game.grid[row][col + 1] == 1) { // Check right cell
        count++;
    }

    return count;
}

function draw() {
    Game.grid.forEach((row, h) => {
        row.forEach((cell, w) => {
            stroke('black')
            fill(cell ? 'black' : 'transparent')
            rect(h * cellSize, w * cellSize, cellSize, cellSize)
        })
    })

    if (Game.state == GameState.paused) return;

    let newGrid = [];

    Game.grid.forEach((row, h) => {
        let newRow = [];

        row.forEach((cell, w) => {
            let newCell = cell;
            let liveNeighbors = countLiveNeighbors(h, w);

            if (cell && (liveNeighbors > 3 || liveNeighbors < 2)) // Dies if is Alive and >3 or <2 live neighbors
            {
                newCell = false
            } 
            else if (!cell && liveNeighbors == 3) // Live if is Dead and has 3 neighbors
            {
                newCell = true
            }

            newRow.push(newCell);
        });

        newGrid.push(newRow);
    });

    Game.grid = newGrid; // Update the grid
}