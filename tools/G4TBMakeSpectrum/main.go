package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"github.com/decibelcooper/proio/go-proio"
	"go-hep.org/x/hep/hbook"
	"go-hep.org/x/hep/hplot"
	"golang.org/x/exp/rand"
	"gonum.org/v1/gonum/stat/distuv"
	//"gonum.org/v1/plot"
	"gonum.org/v1/plot/vg"
)

func printUsage() {
	fmt.Fprintf(os.Stderr,
		`Usage: makeSpectrum [options] <proio-input-file> <output-pdf-file>

options:
`,
	)
	flag.PrintDefaults()
}

func main() {
	flag.Usage = printUsage
	flag.Parse()

	if flag.NArg() < 2 {
		printUsage()
		log.Fatal("Invalid Arguments")
	}

	reader, err := proio.Open(flag.Arg(0))
	if err != nil {
		log.Fatal(err)
	}
	defer reader.Close()

	RNG := rand.New(rand.NewSource(0))
	poiss := &distuv.Poisson{Lambda: 4.0,
		Src: RNG,
	}

	hist := hbook.NewH1D(100, 1, 4500)
	for event := range reader.ScanEvents() {
		simIDs := event.TaggedEntries("Simulated")
		var totCharge float64
		for range simIDs {
			totCharge += poiss.Rand()
		}
		hist.Fill(totCharge, 1.)
	}

	p := hplot.New()
	histPlot := hplot.NewH1D(hist)
	p.Add(histPlot)

	if err := p.Save(4*vg.Inch, 3*vg.Inch, flag.Arg(1)); err != nil {
		log.Fatalf("error saving plot: %v\n", err)
	}

}
