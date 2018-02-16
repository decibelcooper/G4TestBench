package main

import (
	"flag"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"

	"github.com/decibelcooper/proio/go-proio"
	"go-hep.org/x/hep/hbook"
	"go-hep.org/x/hep/hplot"
	"golang.org/x/exp/rand"
	"gonum.org/v1/gonum/stat/distuv"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/vg"
)

var (
	nBins = flag.Int("n", 100, "number of bins")
	min   = flag.Float64("min", 1, "minimum x value")
	max   = flag.Float64("max", 4500, "maximum x value")
)

func printUsage() {
	fmt.Fprintf(os.Stderr,
		`Usage: G4TBMakeSpectrum [options] <proio-input-file> <output-pdf-file>

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

	hist := hbook.NewH1D(*nBins, *min, *max)
	for event := range reader.ScanEvents() {
		simIDs := event.TaggedEntries("Simulated")
		var totCharge float64
		for range simIDs {
			totCharge += poiss.Rand()
		}
		hist.Fill(totCharge, 1.)
	}

	p := hplot.New()
	p.Y.Tick.Marker = PreciseTicks{}
	p.X.Tick.Marker = PreciseTicks{}
	histPlot := hplot.NewH1D(hist)
	histPlot.Infos.Style = hplot.HInfoSummary
	p.Add(histPlot)

	if err := p.Save(4*vg.Inch, 3*vg.Inch, flag.Arg(1)); err != nil {
		log.Fatalf("error saving plot: %v\n", err)
	}

}

type PreciseTicks struct{}

func (PreciseTicks) Ticks(min, max float64) []plot.Tick {
	const suggestedTicks = 4

	if max <= min {
		panic("illegal range")
	}

	tens := math.Pow10(int(math.Floor(math.Log10(max - min))))
	n := (max - min) / tens
	for n < suggestedTicks-1 {
		tens /= 10
		n = (max - min) / tens
	}

	majorMult := int(n / (suggestedTicks - 1))
	switch majorMult {
	case 7:
		majorMult = 6
	case 9:
		majorMult = 8
	}
	majorDelta := float64(majorMult) * tens
	val := math.Floor(min/majorDelta) * majorDelta
	// Makes a list of non-truncated y-values.
	var labels []float64
	for val <= max {
		if val >= min {
			labels = append(labels, val)
		}
		val += majorDelta
	}
	prec := int(math.Ceil(math.Log10(val)) - math.Floor(math.Log10(majorDelta)))
	// Makes a list of big ticks.
	var ticks []plot.Tick
	for _, v := range labels {
		vRounded := round(v, prec)
		ticks = append(ticks, plot.Tick{Value: vRounded, Label: formatFloatTick(vRounded, -1)})
	}
	minorDelta := majorDelta / 2
	switch majorMult {
	case 3, 6:
		minorDelta = majorDelta / 3
	case 5:
		minorDelta = majorDelta / 5
	}

	val = math.Floor(min/minorDelta) * minorDelta
	for val <= max {
		found := false
		for _, t := range ticks {
			if t.Value == val {
				found = true
			}
		}
		if val >= min && val <= max && !found {
			ticks = append(ticks, plot.Tick{Value: val})
		}
		val += minorDelta
	}
	return ticks
}

func round(x float64, prec int) float64 {
	if x == 0 {
		// Make sure zero is returned
		// without the negative bit set.
		return 0
	}
	// Fast path for positive precision on integers.
	if prec >= 0 && x == math.Trunc(x) {
		return x
	}
	pow := math.Pow10(prec)
	intermed := x * pow
	if math.IsInf(intermed, 0) {
		return x
	}
	if x < 0 {
		x = math.Ceil(intermed - 0.5)
	} else {
		x = math.Floor(intermed + 0.5)
	}

	if x == 0 {
		return 0
	}

	return x / pow
}

func formatFloatTick(v float64, prec int) string {
	return strconv.FormatFloat(v, 'g', prec, 64)
}
