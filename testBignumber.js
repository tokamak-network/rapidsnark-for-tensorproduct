const { ethers } = require("ethers");

function main () {
  const a = ethers.BigNumber.from([1,1,1,1])
  const b = ethers.BigNumber.from([2,2,2,2])
  console.log(a, a.toString())
  console.log(b, b.toString())
  console.log(a.mul(b), a.mul(b).toString())
}

main()