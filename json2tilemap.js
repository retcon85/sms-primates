const data = require('./assets/background.generated.json')
const { Buffer } = require('node:buffer')

const tilemap = new Array(24).fill(0).map(_ => new Array(32).fill(0).map(_ => [null, null]))

const { meta: { size: { w } } , frames } = data
frames.forEach(({ filename, spriteSourceSize: { x: dstX, y: dstY }, frame: { x: srcX, y: srcY } }) => {
  const entry = tilemap[dstY / 8][dstX / 8]
  const loc = (srcX / 8) + ((w / 8) * (srcY / 8))
  const isForeground = !filename.startsWith('BG-')
  if (!isForeground || loc !== 1) { // hack: transparency tile needs to be in location 1 for this to work
    entry[0] = loc
    entry[1] = isForeground
  }
})

tilemap.forEach((row) => {
  row.forEach((col) => {
    const buffer = Buffer.alloc(2)
    buffer[0] = col[0] % 256
    buffer[1] = (col[0] >= 256 ? 1 : 0) | (col[1] ? 16 : 0)
    process.stdout.write(buffer, 'binary')
  })
})
