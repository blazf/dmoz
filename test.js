"use strict"


const Dmoz = require("./");
const dmoz = new Dmoz({
    bow: "./data/Top.Bow",
    bowPart: "./data/Top.BowPart",
    filter: "./data/filter.txt",
    classifier: "./data/classifier.dat"
});

let texts = [
    "The European Central Bank said Banca Monte dei Paschi di Siena SpA needs about 8.8 billion euros ($9.2 billion) to bolster its balance sheet, almost twice the amount the Italian lender had sought to raise in a failed capital increase. The calculation is based on the results of a 2016 stress test, the Italian bank said in a statement late Monday, citing two letters from the ECB. While the ECB saw worsening liquidity at Monte Paschi between Nov. 30 and Dec. 21, it still considers the Italian bank to be solvent. The lender is seeking additional information on the central bank’s calculations. The Italian government said Friday it will plow as much as 20 billion euros into Monte Paschi and other banks after the lender failed in its plan to raise about 5 billion euros from the market. Chief Executive Officer Marco Morelli had crisscrossed the globe looking for investors to back the bank’s reorganization plan, which included a share sale, a debt-for-equity swap and the sale of 28 billion of soured loans. Italy plans to invest 6.3 billion euros in the bank, daily Il Sole 24 Ore reported Tuesday, a day after the newspaper said that the European Central Bank had called for a 4.5 billion-euro contribution from the Italian state and 4.3 billion euros from bondholders. A government spokesman declined to comment on the matter.",
    "Stanford University, located between San Francisco and San Jose in the heart of California's Silicon Valley, is one of the world's leading teaching and research universities. Since its opening in 1891, Stanford has been dedicated to finding solutions to big challenges and to preparing students for leadership in a complex world"
]

for (let text of texts) {
    console.log(dmoz.classify(text));
}
