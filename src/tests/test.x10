public static void main(String[] args) {
    final int n = 100;
    final BoxedDouble oddSum = new BoxedDouble();
    double evenSum = 0;
    finish {
        async {
             // Compute oddSum in child activity
             for(inti=1;i<=n;i+=2)
                oddSum.val += i;
        }
        // Compute evenSum in parent activity
        for(intj=2;j<=n;j+=2)
            evenSum += j;
    } // finish
        System.out.println("Sum = " + (oddSum.val + evenSum));
} // main()