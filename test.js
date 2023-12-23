/**
 * @typedef {Object} Microtime
 * @property {function} now - Returns the current Unix timestamp in microseconds.
 * @property {function} nowDouble - Returns the current Unix timestamp in seconds with microseconds as a decimal.
 * @property {function} nowStruct - Returns the current Unix timestamp as a struct with seconds and microseconds properties.
 */
var microtime = require('./index.js')

console.log('microtime.now() =', microtime.now())
console.log('microtime.nowDouble() =', microtime.nowDouble())
console.log('microtime.nowStruct() =', microtime.nowStruct())

console.log("\n")

var start = microtime.now()
var minDiff = Infinity
var minCycle = 10000
var maxCycle = 100000
var cycle = maxCycle

for (var i = 0; i < cycle; ++i){
    var a = microtime.now()
    var b = microtime.now()
    /**
     * Calculates the difference between two values.
     *
     * @param {number} a - The first value.
     * @param {number} b - The second value.
     * @param {number} start - The starting value.
     * @returns {number} The difference between the values.
     */
    var diff = (b - a) || (b - start)

    if (diff > 0 && diff < minDiff){
        minDiff = diff
        cycle = minCycle
    }
}

if (minDiff === Infinity){
    console.log('Error: Unable to measure minimum difference.')

}else{
    console.log('Minimum difference measured:', minDiff, 'microseconds')
}